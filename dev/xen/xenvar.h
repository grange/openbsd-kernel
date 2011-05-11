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

#ifndef _DEV_XEN_XENVAR_H_
#define _DEV_XEN_XENVAR_H_

/*
 * Xen paravirtual drivers interface.
 */

#define XEN_DEBUG	/* XXX: remove on release */

/* Debug levels */
#define XEN_D_ERR	0x0001
#define XEN_D_INFO	0x0002
#define XEN_D_EVT	0x0004
#define XEN_D_ALL	0xffff

#ifdef XEN_DEBUG
extern int xen_debug;
#define DPRINTF(a, b)	do { if (xen_debug & (a)) printf b; } while (0)
#else
#define DPRINTF(a, b)
#endif

struct xen_softc {
	struct device	sc_dev;

	/* store */
	int		sc_st_ec;	/* event chan */
	paddr_t		sc_st_pa;	/* phys addr */
	volatile struct xenstore_domain_interface *sc_st_if; /* rings */
};

struct xen_attach_args {
	const char *	xa_name;
	char		xa_node[64];

	struct xen_softc *xa_xen;
};

/* xen.c */
int xen_attach(struct xen_softc *);
int xen_intr(void *);

/* xen_subr.c */
int xen_store_list(struct xen_softc *, const char *, char **, int *);
int xen_store_read(struct xen_softc *, const char *, const char *,
    char *, int, int *);
#define xen_store_reads(sc, node, item, buf, size) \
    xen_store_read((sc), (node), (item), (buf), (size), NULL)
#define xen_store_readi(sc, node, item, valp) \
    xen_store_read((sc), (node), (item), NULL, 0, (valp))
int xen_atoi(const char *);

/* XXX: amd64 and i386 don't have unified cpuid() function yet */
static inline void
_cpuid(u_int32_t code, u_int32_t *regs)
{
	__asm __volatile("cpuid"
	    : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
	    : "a" (code));
}

#endif	/* !_DEV_XEN_XENVAR_H_ */
