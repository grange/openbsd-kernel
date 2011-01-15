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
 * Xen platform device PCI attachment.
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/device.h>

#include <machine/bus.h>

#include <dev/pci/pcidevs.h>
#include <dev/pci/pcireg.h>
#include <dev/pci/pcivar.h>

#include <dev/xen/xenvar.h>

#define XEN_PCI_MEMREG		0x14		/* mem space BAR */
#define XEN_PCI_MEMSIZE		0x01000000	/* mem space max size */

struct xen_pci_softc {
	struct xen_softc	sc_xen;

	bus_space_tag_t		sc_memt;
	bus_space_handle_t	sc_memh;
};

int	xen_pci_match(struct device *, void *, void *);
void	xen_pci_attach(struct device *, struct device *, void *);

struct cfattach xen_ca = {
	sizeof(struct xen_pci_softc),
	xen_pci_match,
	xen_pci_attach
};

struct cfdriver xen_cd = {
	NULL, "xen", DV_DULL
};

static const struct pci_matchid xen_pci_ids[] = {
	{ PCI_VENDOR_XENSOURCE,	PCI_PRODUCT_XENSOURCE_PLATFORMDEV }
};

int
xen_pci_match(struct device *parent, void *match, void *aux)
{
	return (pci_matchbyid(aux, xen_pci_ids,
	    sizeof(xen_pci_ids) / sizeof(xen_pci_ids[0])));
}

void
xen_pci_attach(struct device *parent, struct device *self, void *aux)
{
	struct xen_pci_softc *sc = (struct xen_pci_softc *)self;
	struct pci_attach_args *pa = aux;
	bus_size_t membase, memsize;
	pci_intr_handle_t ih;
	const char *intrstr;
	void *ic;

	/* Map memory space */
	if (pci_mapreg_map(pa, XEN_PCI_MEMREG, PCI_MAPREG_TYPE_MEM,
	    0, &sc->sc_memt, &sc->sc_memh, &membase, &memsize,
	    XEN_PCI_MEMSIZE)) {
		printf(": can't map mem space\n");
		return;
	}

	/* Install interrupt handler */
	if (pci_intr_map(pa, &ih)) {
		printf(": can't map interrupt\n");
		goto fail1;
	}
	intrstr = pci_intr_string(pa->pa_pc, ih);
	/* XXX: IPL_VM is for baloon driver */
	if ((ic = pci_intr_establish(pa->pa_pc, ih, IPL_VM, xen_intr, sc,
	    self->dv_xname)) == NULL) {
		printf(": can't establish interrupt");
		if (intrstr != NULL)
			printf(" at %s", intrstr);
		printf("\n");
		goto fail1;
	}
	printf(": %s\n", intrstr);

	printf("%s", self->dv_xname);
	if (xen_attach(&sc->sc_xen))
		goto fail2;

	return;
fail2:
	pci_intr_disestablish(pa->pa_pc, ic);
fail1:
	bus_space_unmap(sc->sc_memt, sc->sc_memh, memsize);
}
