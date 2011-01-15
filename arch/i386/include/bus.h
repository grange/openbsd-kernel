/*	$OpenBSD: bus.h,v 1.53 2010/09/06 19:05:48 kettenis Exp $	*/
/*	$NetBSD: bus.h,v 1.6 1996/11/10 03:19:25 thorpej Exp $	*/

/*-
 * Copyright (c) 1996, 1997 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe of the Numerical Aerospace Simulation Facility,
 * NASA Ames Research Center.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Copyright (c) 1996 Charles M. Hannum.  All rights reserved.
 * Copyright (c) 1996 Jason R. Thorpe.  All rights reserved.
 * Copyright (c) 1996 Christopher G. Demetriou.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Christopher G. Demetriou
 *	for the NetBSD Project.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _I386_BUS_H_
#define _I386_BUS_H_

#include <sys/mutex.h>
#include <sys/tree.h>

#include <machine/pio.h>

/*
 * Values for the i386 bus space tag, not to be used directly by MI code.
 */
#define	I386_BUS_SPACE_IO	0	/* space is i/o space */
#define I386_BUS_SPACE_MEM	1	/* space is mem space */

/*
 * Bus address and size types
 */
typedef u_long bus_addr_t;
typedef u_long bus_size_t;

/*
 * Access methods for bus resources and address space.
 */
typedef	int bus_space_tag_t;
typedef	u_long bus_space_handle_t;

int	bus_space_map(bus_space_tag_t t, bus_addr_t addr,
	    bus_size_t size, int flags, bus_space_handle_t *bshp);
/* like bus_space_map(), but without extent map checking/allocation */
int	_bus_space_map(bus_space_tag_t t, bus_addr_t addr,
	    bus_size_t size, int flags, bus_space_handle_t *bshp);
void	bus_space_unmap(bus_space_tag_t t, bus_space_handle_t bsh,
	    bus_size_t size);
/* like bus_space_unmap(), but without extent map deallocation */
void	_bus_space_unmap(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, bus_addr_t *);
int	bus_space_subregion(bus_space_tag_t t, bus_space_handle_t bsh,
	    bus_size_t offset, bus_size_t size, bus_space_handle_t *nbshp);

int	bus_space_alloc(bus_space_tag_t t, bus_addr_t rstart,
	    bus_addr_t rend, bus_size_t size, bus_size_t align,
	    bus_size_t boundary, int flags, bus_addr_t *addrp,
	    bus_space_handle_t *bshp);
void	bus_space_free(bus_space_tag_t t, bus_space_handle_t bsh,
	    bus_size_t size);

/*
 *	u_intN_t bus_space_read_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset);
 *
 * Read a 1, 2, 4, or 8 byte quantity from bus space
 * described by tag/handle/offset.
 */
u_int8_t	bus_space_read_1(bus_space_tag_t, bus_space_handle_t,
		    bus_size_t);

u_int16_t	bus_space_read_2(bus_space_tag_t, bus_space_handle_t,
		    bus_size_t);

u_int32_t	bus_space_read_4(bus_space_tag_t, bus_space_handle_t,
		    bus_size_t);

#if 0	/* Cause a link error for bus_space_read_8 */
#define	bus_space_read_8(t, h, o)	!!! bus_space_read_8 unimplemented !!!
#endif

#if 0	/* Cause a link error for bus_space_read_8 */
#define	bus_space_read_8(t, h, o)	!!! bus_space_read_8 unimplemented !!!
#endif

/*
 *	void bus_space_read_multi_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    u_intN_t *addr, size_t count);
 *
 * Read `count' 1, 2, 4, or 8 byte quantities from bus space
 * described by tag/handle/offset and copy into buffer provided.
 */

#define	bus_space_read_raw_multi_2(t, h, o, a, c) \
    bus_space_read_multi_2((t), (h), (o), (u_int16_t *)(a), (c) >> 1)
#define	bus_space_read_raw_multi_4(t, h, o, a, c) \
    bus_space_read_multi_4((t), (h), (o), (u_int32_t *)(a), (c) >> 2)

void	bus_space_read_multi_1(bus_space_tag_t, bus_space_handle_t, bus_size_t,
	    u_int8_t *, bus_size_t);
void	bus_space_read_multi_2(bus_space_tag_t, bus_space_handle_t, bus_size_t,
	    u_int16_t *, bus_size_t);
void	bus_space_read_multi_4(bus_space_tag_t, bus_space_handle_t, bus_size_t,
	    u_int32_t *, bus_size_t);

#if 0	/* Cause a link error for bus_space_read_multi_8 */
#define	bus_space_read_multi_8	!!! bus_space_read_multi_8 unimplemented !!!
#endif

/*
 *	void bus_space_read_raw_multi_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    u_int8_t *addr, size_t count);
 *
 * Read `count' bytes in 2, 4 or 8 byte wide quantities from bus space
 * described by tag/handle/offset and copy into buffer provided.  The buffer
 * must have proper alignment for the N byte wide entities.  Furthermore
 * possible byte-swapping should be done by these functions.
 */

#define	bus_space_read_raw_multi_2(t, h, o, a, c) \
    bus_space_read_multi_2((t), (h), (o), (u_int16_t *)(a), (c) >> 1)
#define	bus_space_read_raw_multi_4(t, h, o, a, c) \
    bus_space_read_multi_4((t), (h), (o), (u_int32_t *)(a), (c) >> 2)

#if 0	/* Cause a link error for bus_space_read_raw_multi_8 */
#define	bus_space_read_raw_multi_8 \
    !!! bus_space_read_raw_multi_8 unimplemented !!!
#endif

/*
 *	void bus_space_read_region_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    u_intN_t *addr, size_t count);
 *
 * Read `count' 1, 2, 4, or 8 byte quantities from bus space
 * described by tag/handle and starting at `offset' and copy into
 * buffer provided.
 */

void	bus_space_read_region_1(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, u_int8_t *, bus_size_t);
void	bus_space_read_region_2(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, u_int16_t *, bus_size_t);
void	bus_space_read_region_4(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, u_int32_t *, bus_size_t);

#if 0	/* Cause a link error for bus_space_read_region_8 */
#define	bus_space_read_region_8	!!! bus_space_read_region_8 unimplemented !!!
#endif

/*
 *	void bus_space_read_raw_region_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    u_int8_t *addr, size_t count);
 *
 * Read `count' bytes in 2, 4 or 8 byte wide quantities from bus space
 * described by tag/handle and starting at `offset' and copy into
 * buffer provided.  The buffer must have proper alignment for the N byte
 * wide entities.  Furthermore possible byte-swapping should be done by
 * these functions.
 */

#define	bus_space_read_raw_region_2(t, h, o, a, c) \
    bus_space_read_region_2((t), (h), (o), (u_int16_t *)(a), (c) >> 1)
#define	bus_space_read_raw_region_4(t, h, o, a, c) \
    bus_space_read_region_4((t), (h), (o), (u_int32_t *)(a), (c) >> 2)

#if 0	/* Cause a link error for bus_space_read_raw_region_8 */
#define	bus_space_read_raw_region_8 \
    !!! bus_space_read_raw_region_8 unimplemented !!!
#endif

/*
 *	void bus_space_write_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    u_intN_t value);
 *
 * Write the 1, 2, 4, or 8 byte value `value' to bus space
 * described by tag/handle/offset.
 */

void	bus_space_write_1(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, u_int8_t);
void	bus_space_write_2(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, u_int16_t);
void	bus_space_write_4(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, u_int32_t);

#if 0	/* Cause a link error for bus_space_write_8 */
#define	bus_space_write_8	!!! bus_space_write_8 not implemented !!!
#endif

/*
 *	void bus_space_write_multi_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    const u_intN_t *addr, size_t count);
 *
 * Write `count' 1, 2, 4, or 8 byte quantities from the buffer
 * provided to bus space described by tag/handle/offset.
 */

void	bus_space_write_multi_1(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, const u_int8_t *, bus_size_t);
void	bus_space_write_multi_2(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, const u_int16_t *, bus_size_t);
void	bus_space_write_multi_4(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, const u_int32_t *, bus_size_t);

#if 0	/* Cause a link error for bus_space_write_multi_8 */
#define	bus_space_write_multi_8(t, h, o, a, c)				\
			!!! bus_space_write_multi_8 unimplemented !!!
#endif

/*
 *	void bus_space_write_raw_multi_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    const u_int8_t *addr, size_t count);
 *
 * Write `count' bytes in 2, 4 or 8 byte wide quantities from the buffer
 * provided to bus space described by tag/handle/offset.  The buffer
 * must have proper alignment for the N byte wide entities.  Furthermore
 * possible byte-swapping should be done by these functions.
 */

#define	bus_space_write_raw_multi_2(t, h, o, a, c) \
    bus_space_write_multi_2((t), (h), (o), (const u_int16_t *)(a), (c) >> 1)
#define	bus_space_write_raw_multi_4(t, h, o, a, c) \
    bus_space_write_multi_4((t), (h), (o), (const u_int32_t *)(a), (c) >> 2)

#if 0	/* Cause a link error for bus_space_write_raw_multi_8 */
#define	bus_space_write_raw_multi_8 \
    !!! bus_space_write_raw_multi_8 unimplemented !!!
#endif

/*
 *	void bus_space_write_region_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    const u_intN_t *addr, size_t count);
 *
 * Write `count' 1, 2, 4, or 8 byte quantities from the buffer provided
 * to bus space described by tag/handle starting at `offset'.
 */

void	bus_space_write_region_1(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, const u_int8_t *, bus_size_t);
void	bus_space_write_region_2(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, const u_int16_t *, bus_size_t);
void	bus_space_write_region_4(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, const u_int32_t *, bus_size_t);

#if 0	/* Cause a link error for bus_space_write_region_8 */
#define	bus_space_write_region_8					\
			!!! bus_space_write_region_8 unimplemented !!!
#endif

/*
 *	void bus_space_write_raw_region_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    const u_int8_t *addr, size_t count);
 *
 * Write `count' bytes in 2, 4 or 8 byte wide quantities to bus space
 * described by tag/handle and starting at `offset' from the
 * buffer provided.  The buffer must have proper alignment for the N byte
 * wide entities.  Furthermore possible byte-swapping should be done by
 * these functions.
 */

#define	bus_space_write_raw_region_2(t, h, o, a, c) \
    bus_space_write_region_2((t), (h), (o), (const u_int16_t *)(a), (c) >> 1)
#define	bus_space_write_raw_region_4(t, h, o, a, c) \
    bus_space_write_region_4((t), (h), (o), (const u_int32_t *)(a), (c) >> 2)

#if 0	/* Cause a link error for bus_space_write_raw_region_8 */
#define	bus_space_write_raw_region_8 \
    !!! bus_space_write_raw_region_8 unimplemented !!!
#endif

/*
 *	void bus_space_set_multi_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    u_intN_t val, size_t count);
 *
 * Write the 1, 2, 4, or 8 byte value `val' to bus space described
 * by tag/handle/offset `count' times.
 */

void	bus_space_set_multi_1(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, u_int8_t, size_t);
void	bus_space_set_multi_2(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, u_int16_t, size_t);
void	bus_space_set_multi_4(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, u_int32_t, size_t);

#if 0	/* Cause a link error for bus_space_set_multi_8 */
#define	bus_space_set_multi_8					\
			!!! bus_space_set_multi_8 unimplemented !!!
#endif

/*
 *	void bus_space_set_region_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    u_intN_t val, size_t count);
 *
 * Write `count' 1, 2, 4, or 8 byte value `val' to bus space described
 * by tag/handle starting at `offset'.
 */
void	bus_space_set_region_1(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, u_int8_t, size_t);
void	bus_space_set_region_2(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, u_int16_t, size_t);
void	bus_space_set_region_4(bus_space_tag_t, bus_space_handle_t,
	    bus_size_t, u_int32_t, size_t);


#if 0	/* Cause a link error for bus_space_set_region_8 */
#define	bus_space_set_region_8					\
			!!! bus_space_set_region_8 unimplemented !!!
#endif

/*
 *	void bus_space_copy_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh1, bus_size_t off1,
 *	    bus_space_handle_t bsh2, bus_size_t off2,
 *	    size_t count);
 *
 * Copy `count' 1, 2, 4, or 8 byte values from bus space starting
 * at tag/bsh1/off1 to bus space starting at tag/bsh2/off2.
 */

void	bus_space_copy_1(bus_space_tag_t, bus_space_handle_t, bus_size_t,
	     bus_space_handle_t, bus_size_t, bus_size_t);
void	bus_space_copy_2(bus_space_tag_t, bus_space_handle_t, bus_size_t,
	     bus_space_handle_t, bus_size_t, bus_size_t);
void	bus_space_copy_4(bus_space_tag_t, bus_space_handle_t, bus_size_t,
	     bus_space_handle_t, bus_size_t, bus_size_t);

#if 0	/* Cause a link error for bus_space_copy_8 */
#define	bus_space_copy_8					\
			!!! bus_space_copy_8 unimplemented !!!
#endif

#define	i386_space_copy1(a1, a2, cnt, movs, df)		\
	__asm __volatile(df "\n\trep\n\t" movs :	\
	    "+S" (a1), "+D" (a2), "+c" (cnt)	:: "memory", "cc");

#define	i386_space_copy(a1, a2, sz, cnt) do {				\
	if ((void *)(a1) < (void *)(a2)) {				\
		a1 += ((cnt) - 1) * (sz); a2 += ((cnt) - 1) * (sz);	\
		switch (sz) {						\
		case 1:	i386_space_copy1(a1,a2,cnt,"movsb","std");break;\
		case 2:	i386_space_copy1(a1,a2,cnt,"movsw","std");break;\
		case 4:	i386_space_copy1(a1,a2,cnt,"movsl","std");break;\
		}							\
	} else								\
		switch (sz) {						\
		case 1:	i386_space_copy1(a1,a2,cnt,"movsb","cld");break;\
		case 2:	i386_space_copy1(a1,a2,cnt,"movsw","cld");break;\
		case 4:	i386_space_copy1(a1,a2,cnt,"movsl","cld");break;\
		}							\
} while (0)

/*
 * Bus read/write barrier methods.
 *
 *	void bus_space_barrier(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    bus_size_t len, int flags);
 *
 * Note: the i386 does not currently require barriers, but we must
 * provide the flags to MI code.
 */
#define	bus_space_barrier(t, h, o, l, f)	\
	((void)((void)(t), (void)(h), (void)(o), (void)(l), (void)(f)))
#define	BUS_SPACE_BARRIER_READ	0x01		/* force read barrier */
#define	BUS_SPACE_BARRIER_WRITE	0x02		/* force write barrier */

#define	BUS_SPACE_MAP_CACHEABLE		0x0001
#define	BUS_SPACE_MAP_LINEAR		0x0002
#define	BUS_SPACE_MAP_PREFETCHABLE	0x0008

/*
 *	void *bus_space_vaddr(bus_space_tag_t, bus_space_handle_t);
 *
 * Get the kernel virtual address for the mapped bus space.
 * Only allowed for regions mapped with BUS_SPACE_MAP_LINEAR.
 */
#define bus_space_vaddr(t, h) \
	((t) == I386_BUS_SPACE_IO ? (void *)(NULL) : (void *)(h))

/*
 * Flags used in various bus DMA methods.
 */
#define	BUS_DMA_WAITOK		0x0000	/* safe to sleep (pseudo-flag) */
#define	BUS_DMA_NOWAIT		0x0001	/* not safe to sleep */
#define	BUS_DMA_ALLOCNOW	0x0002	/* perform resource allocation now */
#define	BUS_DMA_COHERENT	0x0004	/* hint: map memory DMA coherent */
#define	BUS_DMA_BUS1		0x0010	/* placeholders for bus functions... */
#define	BUS_DMA_BUS2		0x0020
#define	BUS_DMA_BUS3		0x0040
#define	BUS_DMA_24BIT		0x0080	/* isadma map */
#define	BUS_DMA_STREAMING	0x0100	/* hint: sequential, unidirectional */
#define	BUS_DMA_READ		0x0200	/* mapping is device -> memory only */
#define	BUS_DMA_WRITE		0x0400	/* mapping is memory -> device only */
#define	BUS_DMA_NOCACHE		0x0800	/* map memory uncached */
#define	BUS_DMA_ZERO		0x1000	/* dmamem_alloc return zeroed mem */
#define	BUS_DMA_SG		0x2000	/* Internal. memory is for SG map */

/* types for _dm_buftype */
#define	BUS_BUFTYPE_INVALID	0
#define	BUS_BUFTYPE_LINEAR	1
#define	BUS_BUFTYPE_MBUF	2
#define	BUS_BUFTYPE_UIO		3
#define	BUS_BUFTYPE_RAW		4

/* Forwards needed by prototypes below. */
struct mbuf;
struct proc;
struct uio;

/*
 * Operations performed by bus_dmamap_sync().
 */
#define BUS_DMASYNC_PREREAD	0x01
#define BUS_DMASYNC_POSTREAD	0x02
#define BUS_DMASYNC_PREWRITE	0x04
#define BUS_DMASYNC_POSTWRITE	0x08

typedef struct bus_dma_tag		*bus_dma_tag_t;
typedef struct bus_dmamap		*bus_dmamap_t;

/*
 *	bus_dma_segment_t
 *
 *	Describes a single contiguous DMA transaction.  Values
 *	are suitable for programming into DMA registers.
 */
struct bus_dma_segment {
	bus_addr_t	ds_addr;	/* DMA address */
	bus_size_t	ds_len;		/* length of transfer */
	/*
	 * Ugh. need this so can pass alignment down from bus_dmamem_alloc
	 * to scatter gather maps. only the first one is used so the rest is
	 * wasted space. bus_dma could do with fixing the api for this.
	 */
	 bus_size_t	_ds_boundary;	/* don't cross */
	 bus_size_t	_ds_align;	/* align to me */
};
typedef struct bus_dma_segment	bus_dma_segment_t;

/*
 *	bus_dma_tag_t
 *
 *	A machine-dependent opaque type describing the implementation of
 *	DMA for a given bus.
 */

struct bus_dma_tag {
	void	*_cookie;		/* cookie used in the guts */

	/*
	 * DMA mapping methods.
	 */
	int	(*_dmamap_create)(bus_dma_tag_t, bus_size_t, int,
		    bus_size_t, bus_size_t, int, bus_dmamap_t *);
	void	(*_dmamap_destroy)(bus_dma_tag_t, bus_dmamap_t);
	int	(*_dmamap_load)(bus_dma_tag_t, bus_dmamap_t, void *,
		    bus_size_t, struct proc *, int);
	int	(*_dmamap_load_mbuf)(bus_dma_tag_t, bus_dmamap_t,
		    struct mbuf *, int);
	int	(*_dmamap_load_uio)(bus_dma_tag_t, bus_dmamap_t,
		    struct uio *, int);
	int	(*_dmamap_load_raw)(bus_dma_tag_t, bus_dmamap_t,
		    bus_dma_segment_t *, int, bus_size_t, int);
	void	(*_dmamap_unload)(bus_dma_tag_t, bus_dmamap_t);
	void	(*_dmamap_sync)(bus_dma_tag_t, bus_dmamap_t,
		    bus_addr_t, bus_size_t, int);

	/*
	 * DMA memory utility functions.
	 */
	int	(*_dmamem_alloc)(bus_dma_tag_t, bus_size_t, bus_size_t,
		    bus_size_t, bus_dma_segment_t *, int, int *, int);
	void	(*_dmamem_free)(bus_dma_tag_t,
		    bus_dma_segment_t *, int);
	int	(*_dmamem_map)(bus_dma_tag_t, bus_dma_segment_t *,
		    int, size_t, caddr_t *, int);
	void	(*_dmamem_unmap)(bus_dma_tag_t, caddr_t, size_t);
	paddr_t	(*_dmamem_mmap)(bus_dma_tag_t, bus_dma_segment_t *,
		    int, off_t, int, int);
};

#define	bus_dmamap_create(t, s, n, m, b, f, p)			\
	(*(t)->_dmamap_create)((t), (s), (n), (m), (b), (f), (p))
#define	bus_dmamap_destroy(t, p)				\
	(*(t)->_dmamap_destroy)((t), (p))
#define	bus_dmamap_load(t, m, b, s, p, f)			\
	(*(t)->_dmamap_load)((t), (m), (b), (s), (p), (f))
#define	bus_dmamap_load_mbuf(t, m, b, f)			\
	(*(t)->_dmamap_load_mbuf)((t), (m), (b), (f))
#define	bus_dmamap_load_uio(t, m, u, f)				\
	(*(t)->_dmamap_load_uio)((t), (m), (u), (f))
#define	bus_dmamap_load_raw(t, m, sg, n, s, f)			\
	(*(t)->_dmamap_load_raw)((t), (m), (sg), (n), (s), (f))
#define	bus_dmamap_unload(t, p)					\
	(*(t)->_dmamap_unload)((t), (p))
#define	bus_dmamap_sync(t, p, o, l, ops)			\
	(*(t)->_dmamap_sync)((t), (p), (o), (l), (ops))

#define	bus_dmamem_alloc(t, s, a, b, sg, n, r, f)		\
	(*(t)->_dmamem_alloc)((t), (s), (a), (b), (sg), (n), (r), (f))
#define	bus_dmamem_free(t, sg, n)				\
	(*(t)->_dmamem_free)((t), (sg), (n))
#define	bus_dmamem_map(t, sg, n, s, k, f)			\
	(*(t)->_dmamem_map)((t), (sg), (n), (s), (k), (f))
#define	bus_dmamem_unmap(t, k, s)				\
	(*(t)->_dmamem_unmap)((t), (k), (s))
#define	bus_dmamem_mmap(t, sg, n, o, p, f)			\
	(*(t)->_dmamem_mmap)((t), (sg), (n), (o), (p), (f))

/*
 *	bus_dmamap_t
 *
 *	Describes a DMA mapping.
 */
struct bus_dmamap {
	/*
	 * PRIVATE MEMBERS: not for use by machine-independent code.
	 */
	bus_size_t	_dm_size;	/* largest DMA transfer mappable */
	int		_dm_segcnt;	/* number of segs this map can map */
	bus_size_t	_dm_maxsegsz;	/* largest possible segment */
	bus_size_t	_dm_boundary;	/* don't cross this */
	int		_dm_flags;	/* misc. flags */

	void		*_dm_cookie;	/* cookie for bus-specific functions */

	/*
	 * PUBLIC MEMBERS: these are used by machine-independent code.
	 */
	bus_size_t	dm_mapsize;	/* size of the mapping */
	int		dm_nsegs;	/* # valid segments in mapping */
	bus_dma_segment_t dm_segs[1];	/* segments; variable length */
};

int	_bus_dmamap_create(bus_dma_tag_t, bus_size_t, int, bus_size_t,
	    bus_size_t, int, bus_dmamap_t *);
void	_bus_dmamap_destroy(bus_dma_tag_t, bus_dmamap_t);
int	_bus_dmamap_load(bus_dma_tag_t, bus_dmamap_t, void *,
	    bus_size_t, struct proc *, int);
int	_bus_dmamap_load_mbuf(bus_dma_tag_t, bus_dmamap_t,
	    struct mbuf *, int);
int	_bus_dmamap_load_uio(bus_dma_tag_t, bus_dmamap_t,
	    struct uio *, int);
int	_bus_dmamap_load_raw(bus_dma_tag_t, bus_dmamap_t,
	    bus_dma_segment_t *, int, bus_size_t, int);
void	_bus_dmamap_unload(bus_dma_tag_t, bus_dmamap_t);
void	_bus_dmamap_sync(bus_dma_tag_t, bus_dmamap_t, bus_addr_t,
	    bus_size_t, int);

int	_bus_dmamem_alloc(bus_dma_tag_t tag, bus_size_t size,
	    bus_size_t alignment, bus_size_t boundary,
	    bus_dma_segment_t *segs, int nsegs, int *rsegs, int flags);
void	_bus_dmamem_free(bus_dma_tag_t tag, bus_dma_segment_t *segs,
	    int nsegs);
int	_bus_dmamem_map(bus_dma_tag_t tag, bus_dma_segment_t *segs,
	    int nsegs, size_t size, caddr_t *kvap, int flags);
void	_bus_dmamem_unmap(bus_dma_tag_t tag, caddr_t kva,
	    size_t size);
paddr_t	_bus_dmamem_mmap(bus_dma_tag_t tag, bus_dma_segment_t *segs,
	    int nsegs, off_t off, int prot, int flags);

int	_bus_dmamem_alloc_range(bus_dma_tag_t tag, bus_size_t size,
	    bus_size_t alignment, bus_size_t boundary,
	    bus_dma_segment_t *segs, int nsegs, int *rsegs, int flags,
	    paddr_t low, paddr_t high);

struct extent;

/* Scatter gather bus_dma functions. */
struct sg_cookie {
	struct mutex	 sg_mtx;
	struct extent	*sg_ex;
	void		*sg_hdl;
	void		(*bind_page)(void *, bus_addr_t, paddr_t, int);
	void		(*unbind_page)(void *, bus_addr_t);
	void		(*flush_tlb)(void *);
};

/* 
 * per-map DVMA page table
 */
struct sg_page_entry {
	SPLAY_ENTRY(sg_page_entry)	spe_node;
	paddr_t				spe_pa;
	bus_addr_t			spe_va;
};

/* for sg_dma this will be in the map's dm_cookie. */
struct sg_page_map {
	SPLAY_HEAD(sg_page_tree, sg_page_entry) spm_tree;

	void			*spm_origbuf;	/* pointer to original data */
	int			 spm_buftype;	/* type of data */
	struct proc		*spm_proc;	/* proc that owns the mapping */

	int			 spm_maxpage;	/* Size of allocated page map */
	int			 spm_pagecnt;	/* Number of entries in use */
	bus_addr_t		 spm_start;	/* dva when bound */
	bus_size_t		 spm_size;	/* size of bound map */
	struct sg_page_entry	 spm_map[1];
};

struct sg_cookie	*sg_dmatag_init(char *, void *, bus_addr_t, bus_size_t,
			    void (*)(void *, vaddr_t, paddr_t, int),
			    void (*)(void *, vaddr_t), void (*)(void *));
void	sg_dmatag_destroy(struct sg_cookie *);
int	sg_dmamap_create(bus_dma_tag_t, bus_size_t, int, bus_size_t,
	    bus_size_t, int, bus_dmamap_t *);
void	sg_dmamap_destroy(bus_dma_tag_t, bus_dmamap_t);
void	sg_dmamap_set_alignment(bus_dma_tag_t, bus_dmamap_t, u_long);
int	sg_dmamap_load(bus_dma_tag_t, bus_dmamap_t, void *, bus_size_t,
	    struct proc *, int);
int	sg_dmamap_load_mbuf(bus_dma_tag_t, bus_dmamap_t,
	    struct mbuf *, int);
int	sg_dmamap_load_uio(bus_dma_tag_t, bus_dmamap_t, struct uio *, int);
int	sg_dmamap_load_raw(bus_dma_tag_t, bus_dmamap_t, bus_dma_segment_t *,
	    int, bus_size_t, int);
void	sg_dmamap_unload(bus_dma_tag_t, bus_dmamap_t);
int	sg_dmamap_load_buffer(bus_dma_tag_t, bus_dmamap_t, void *, bus_size_t,
	    struct proc *, int, int *, int);
int	sg_dmamap_load_physarray(bus_dma_tag_t, bus_dmamap_t, paddr_t *,
	    int, int, int *, int);
int	sg_dmamem_alloc(bus_dma_tag_t, bus_size_t, bus_size_t, bus_size_t,
	    bus_dma_segment_t *, int, int *, int);


#endif /* _I386_BUS_H_ */
