/*	$OpenBSD: vmparam.h,v 1.8 2011/03/23 16:54:36 pirofti Exp $ */
/* public domain */
#ifndef _MACHINE_VMPARAM_H_
#define _MACHINE_VMPARAM_H_

#define	VM_PHYSSEG_MAX	32	/* Max number of physical memory segments */

/*
 * On Origin and Octane families, DMA to 32-bit PCI devices is restricted.
 *
 * Systems with physical memory after the 2GB boundary need to ensure
 * memory which may used for DMA transfers is allocated from the low
 * memory range.
 *
 * Other systems, like the O2, do not have such a restriction, but can not
 * have more than 2GB of physical memory, so this doesn't affect them.
 */

#define	VM_NFREELIST		2
#define	VM_FREELIST_DMA32	1	/* memory suitable for 32-bit DMA */

#include <mips64/vmparam.h>

#endif	/* _MACHINE_VMPARAM_H_ */
