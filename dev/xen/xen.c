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
 * Xen platform device driver.
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/device.h>

#include <uvm/uvm_extern.h>

#include <machine/cpufunc.h>
#include <machine/pmap.h>

#include <dev/xen/hypervisor.h>
#include <dev/xen/xenvar.h>

#ifdef XEN_DEBUG
int xen_debug = XEN_D_ALL;	/* XXX: change to XEN_D_ERR on release */
#endif

/* Allocate hypercall page in object file */
__asm(".pushsection .text\n\t"
      ".globl hypercall_page\n\t"
      ".align 4096\n"
      "hypercall_page:\n\t"
      ".skip 4096\n\t"
      ".popsection");

int
xen_attach(struct xen_softc *sc)
{
	u_int32_t regs[4], msr;
	paddr_t pa;

	_cpuid(XEN_CPUID_LEAF(0), regs);
	DPRINTF(XEN_D_INFO, (": leaf 1 0x%08x 0x%08x 0x%08x 0x%08x",
	    regs[0], regs[1], regs[2], regs[3]));
	if (memcmp(regs + 1, "XenVMMXenVMM", 12)) {
		printf(": wrong signature\n");
		return (1);
	}
	if (regs[0] < XEN_CPUID_LEAF(2)) {
		printf(": unsupported\n");
		return (1);
	}

	_cpuid(XEN_CPUID_LEAF(1), regs);
	DPRINTF(XEN_D_INFO, (", leaf 2 0x%08x 0x%08x 0x%08x 0x%08x",
	    regs[0], regs[1], regs[2], regs[3]));
	printf(": ver %d.%d", regs[0] >> 16, regs[0] & 0xffff);

	_cpuid(XEN_CPUID_LEAF(2), regs);
	DPRINTF(XEN_D_INFO, (", leaf 3 0x%08x 0x%08x 0x%08x 0x%08x",
	    regs[0], regs[1], regs[2], regs[3]));
	msr = regs[1];

	/* Pass hypercall page address to hypervisor */
	if (!pmap_extract(pmap_kernel(), (vaddr_t)hypercall_page, &pa)) {
		printf(": can't get hypercall page addr\n");
		return (1);
	}
	wrmsr(msr, pa);

	printf("\n");
	return (0);
}

int
xen_intr(void *arg)
{
	struct xen_softc *sc = arg;

	DPRINTF(XEN_D_EVT, ("%s: intr\n", sc->sc_dev.dv_xname));
	return (1);
}
