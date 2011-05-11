/*	$OpenBSD$	*/

/*
 * Copyright (c) 2011 Alexander Yurchenko <grange@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Xen helper routines.
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/device.h>
#include <sys/malloc.h>

#include <dev/xen/hypervisor.h>
#include <dev/xen/xenvar.h>

int xen_event_send(int);

int
xen_store_list(struct xen_softc *sc, const char *path, char **bufp, int *sizep)
{
	struct xsd_sockmsg msg;
	u_int32_t cons, prod;
	int t;

	msg.type = XS_DIRECTORY;
	msg.req_id = 0;
	msg.tx_id = 0;
	msg.len = strlen(path) + 1;

	/* Write request */
	cons = sc->sc_st_if->req_cons;
	prod = sc->sc_st_if->req_prod;

	memcpy((char *)sc->sc_st_if->req + prod, &msg, sizeof(msg));
	prod += sizeof(msg);
	memcpy((char *)sc->sc_st_if->req + prod, path, msg.len);
	prod += msg.len;

	sc->sc_st_if->req_prod = prod;
	if (xen_event_send(sc->sc_st_ec))
		return (1);

	/* Read response */
	cons = sc->sc_st_if->rsp_cons;
	prod = sc->sc_st_if->rsp_prod;
	for (t = 1000; t && cons == prod; t--) {
		delay(1000);
		prod = sc->sc_st_if->rsp_prod;
	}
	if (!t)
		/* timeout */
		return (1);

	if (prod - cons < sizeof(msg))
		return (1);
	memcpy(&msg, (char *)sc->sc_st_if->rsp + cons, sizeof(msg));
	cons += sizeof(msg);

	if (prod - cons < msg.len)
		return (1);
	if ((*bufp = malloc(msg.len, M_TEMP, M_NOWAIT)) == NULL)
		return (1);
	memcpy(*bufp, (char *)sc->sc_st_if->rsp + cons, msg.len);
	*sizep = msg.len;
	cons += msg.len;

	sc->sc_st_if->rsp_cons = cons;
	xen_event_send(sc->sc_st_ec);

	return (0);
}

int
xen_store_read(struct xen_softc *sc, const char *node, const char *item,
    char *buf, int size, int *valp)
{
	struct xsd_sockmsg msg;
	u_int32_t cons, prod;
	int t;
	char path[64];

	snprintf(path, sizeof(path), "%s/%s", node, item);

	msg.type = XS_READ;
	msg.req_id = 0;
	msg.tx_id = 0;
	msg.len = strlen(path) + 1;

	/* Write request */
	cons = sc->sc_st_if->req_cons;
	prod = sc->sc_st_if->req_prod;

	memcpy((char *)sc->sc_st_if->req + prod, &msg, sizeof(msg));
	prod += sizeof(msg);
	memcpy((char *)sc->sc_st_if->req + prod, path, msg.len);
	prod += msg.len;

	sc->sc_st_if->req_prod = prod;
	if (xen_event_send(sc->sc_st_ec))
		return (1);

	/* Read response */
	cons = sc->sc_st_if->rsp_cons;
	prod = sc->sc_st_if->rsp_prod;
	for (t = 1000; t && cons == prod; t--) {
		delay(1000);
		prod = sc->sc_st_if->rsp_prod;
	}
	if (!t)
		/* timeout */
		return (1);

	if (prod - cons < sizeof(msg))
		return (1);
	memcpy(&msg, (char *)sc->sc_st_if->rsp + cons, sizeof(msg));
	cons += sizeof(msg);

	if (prod - cons < msg.len)
		return (1);

	if (buf)
		memcpy(buf, (char *)sc->sc_st_if->rsp + cons,
		    min(size, msg.len));
	if (valp)
		*valp = xen_atoi((char *)sc->sc_st_if->rsp + cons);
	cons += msg.len;

	sc->sc_st_if->rsp_cons = cons;
	xen_event_send(sc->sc_st_ec);

	return (0);
}

int
xen_event_send(int ec)
{
	struct evtchn_send send = { .port = ec };

	return (HYPERVISOR_event_channel_op(EVTCHNOP_send, &send));
}

int
xen_atoi(const char *s)
{
	int val = 0, mul = 1;
	int i;

	for (i = strlen(s) - 1; i >= 0; i--) {
		val += (s[i] - '0') * mul;
		mul *= 10;
	}

	return val;
}
