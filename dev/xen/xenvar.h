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

	int		sc_store_ec;
	paddr_t		sc_store_pa;
	vaddr_t		sc_store_va;
};

int xen_attach(struct xen_softc *);
int xen_intr(void *);

/* XXX: amd64 and i386 don't have unified cpuid() function yet */
static inline void
_cpuid(u_int32_t code, u_int32_t *regs)
{
	__asm __volatile("cpuid"
	    : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
	    : "a" (code));
}

#endif	/* !_DEV_XEN_XENVAR_H_ */
