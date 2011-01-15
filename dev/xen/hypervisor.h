/*	$OpenBSD$	*/

/*
 * This is a compilation of only necessary excerpts from the
 * Xen public headers which is enough to build paravirtual
 * drivers.
 */

#ifndef _DEV_XEN_HYPERVISOR_H_
#define _DEV_XEN_HYPERVISOR_H_

/*
 * xen.h
 */

/******************************************************************************
 * xen.h
 * 
 * Guest OS interface to Xen.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Copyright (c) 2004, K A Fraser
 */

#ifndef __XEN_PUBLIC_XEN_H__
#define __XEN_PUBLIC_XEN_H__

/*
 * HYPERCALLS
 */

#define __HYPERVISOR_set_trap_table        0
#define __HYPERVISOR_mmu_update            1
#define __HYPERVISOR_set_gdt               2
#define __HYPERVISOR_stack_switch          3
#define __HYPERVISOR_set_callbacks         4
#define __HYPERVISOR_fpu_taskswitch        5
#define __HYPERVISOR_sched_op_compat       6 /* compat since 0x00030101 */
#define __HYPERVISOR_platform_op           7
#define __HYPERVISOR_set_debugreg          8
#define __HYPERVISOR_get_debugreg          9
#define __HYPERVISOR_update_descriptor    10
#define __HYPERVISOR_memory_op            12
#define __HYPERVISOR_multicall            13
#define __HYPERVISOR_update_va_mapping    14
#define __HYPERVISOR_set_timer_op         15
#define __HYPERVISOR_event_channel_op_compat 16 /* compat since 0x00030202 */
#define __HYPERVISOR_xen_version          17
#define __HYPERVISOR_console_io           18
#define __HYPERVISOR_physdev_op_compat    19 /* compat since 0x00030202 */
#define __HYPERVISOR_grant_table_op       20
#define __HYPERVISOR_vm_assist            21
#define __HYPERVISOR_update_va_mapping_otherdomain 22
#define __HYPERVISOR_iret                 23 /* x86 only */
#define __HYPERVISOR_vcpu_op              24
#define __HYPERVISOR_set_segment_base     25 /* x86/64 only */
#define __HYPERVISOR_mmuext_op            26
#define __HYPERVISOR_xsm_op               27
#define __HYPERVISOR_nmi_op               28
#define __HYPERVISOR_sched_op             29
#define __HYPERVISOR_callback_op          30
#define __HYPERVISOR_xenoprof_op          31
#define __HYPERVISOR_event_channel_op     32
#define __HYPERVISOR_physdev_op           33
#define __HYPERVISOR_hvm_op               34
#define __HYPERVISOR_sysctl               35
#define __HYPERVISOR_domctl               36
#define __HYPERVISOR_kexec_op             37
#define __HYPERVISOR_tmem_op              38

/* Architecture-specific hypercall definitions. */
#define __HYPERVISOR_arch_0               48
#define __HYPERVISOR_arch_1               49
#define __HYPERVISOR_arch_2               50
#define __HYPERVISOR_arch_3               51
#define __HYPERVISOR_arch_4               52
#define __HYPERVISOR_arch_5               53
#define __HYPERVISOR_arch_6               54
#define __HYPERVISOR_arch_7               55

/*
 * HYPERCALL COMPATIBILITY.
 */

/* New sched_op hypercall introduced in 0x00030101. */
#if __XEN_INTERFACE_VERSION__ < 0x00030101
#undef __HYPERVISOR_sched_op
#define __HYPERVISOR_sched_op __HYPERVISOR_sched_op_compat
#endif

/* New event-channel and physdev hypercalls introduced in 0x00030202. */
#if __XEN_INTERFACE_VERSION__ < 0x00030202
#undef __HYPERVISOR_event_channel_op
#define __HYPERVISOR_event_channel_op __HYPERVISOR_event_channel_op_compat
#undef __HYPERVISOR_physdev_op
#define __HYPERVISOR_physdev_op __HYPERVISOR_physdev_op_compat
#endif

/* New platform_op hypercall introduced in 0x00030204. */
#if __XEN_INTERFACE_VERSION__ < 0x00030204
#define __HYPERVISOR_dom0_op __HYPERVISOR_platform_op
#endif

/* 
 * VIRTUAL INTERRUPTS
 * 
 * Virtual interrupts that a guest OS may receive from Xen.
 * 
 * In the side comments, 'V.' denotes a per-VCPU VIRQ while 'G.' denotes a
 * global VIRQ. The former can be bound once per VCPU and cannot be re-bound.
 * The latter can be allocated only once per guest: they must initially be
 * allocated to VCPU0 but can subsequently be re-bound.
 */
#define VIRQ_TIMER      0  /* V. Timebase update, and/or requested timeout.  */
#define VIRQ_DEBUG      1  /* V. Request guest to dump debug info.           */
#define VIRQ_CONSOLE    2  /* G. (DOM0) Bytes received on emergency console. */
#define VIRQ_DOM_EXC    3  /* G. (DOM0) Exceptional event for some domain.   */
#define VIRQ_TBUF       4  /* G. (DOM0) Trace buffer has records available.  */
#define VIRQ_DEBUGGER   6  /* G. (DOM0) A domain has paused for debugging.   */
#define VIRQ_XENOPROF   7  /* V. XenOprofile interrupt: new sample available */
#define VIRQ_CON_RING   8  /* G. (DOM0) Bytes received on console            */
#define VIRQ_PCPU_STATE 9  /* G. (DOM0) PCPU state changed                   */
#define VIRQ_MEM_EVENT  10 /* G. (DOM0) A memory event has occured           */

/* Architecture-specific VIRQ definitions. */
#define VIRQ_ARCH_0    16
#define VIRQ_ARCH_1    17
#define VIRQ_ARCH_2    18
#define VIRQ_ARCH_3    19
#define VIRQ_ARCH_4    20
#define VIRQ_ARCH_5    21
#define VIRQ_ARCH_6    22
#define VIRQ_ARCH_7    23

#define NR_VIRQS       24

/* These are passed as 'flags' to update_va_mapping. They can be ORed. */
/* When specifying UVMF_MULTI, also OR in a pointer to a CPU bitmap.   */
/* UVMF_LOCAL is merely UVMF_MULTI with a NULL bitmap pointer.         */
#define UVMF_NONE               (0UL<<0) /* No flushing at all.   */
#define UVMF_TLB_FLUSH          (1UL<<0) /* Flush entire TLB(s).  */
#define UVMF_INVLPG             (2UL<<0) /* Flush only one entry. */
#define UVMF_FLUSHTYPE_MASK     (3UL<<0)
#define UVMF_MULTI              (0UL<<2) /* Flush subset of TLBs. */
#define UVMF_LOCAL              (0UL<<2) /* Flush local TLB.      */
#define UVMF_ALL                (1UL<<2) /* Flush all TLBs.       */

/*
 * Commands to HYPERVISOR_console_io().
 */
#define CONSOLEIO_write         0
#define CONSOLEIO_read          1

/*
 * Commands to HYPERVISOR_vm_assist().
 */
#define VMASST_CMD_enable                0
#define VMASST_CMD_disable               1

/* x86/32 guests: simulate full 4GB segment limits. */
#define VMASST_TYPE_4gb_segments         0

/* x86/32 guests: trap (vector 15) whenever above vmassist is used. */
#define VMASST_TYPE_4gb_segments_notify  1

/*
 * x86 guests: support writes to bottom-level PTEs.
 * NB1. Page-directory entries cannot be written.
 * NB2. Guest must continue to remove all writable mappings of PTEs.
 */
#define VMASST_TYPE_writable_pagetables  2

/* x86/PAE guests: support PDPTs above 4GB. */
#define VMASST_TYPE_pae_extended_cr3     3

#define MAX_VMASST_TYPE                  3

#ifndef __ASSEMBLY__

typedef uint16_t domid_t;

/* Domain ids >= DOMID_FIRST_RESERVED cannot be used for ordinary domains. */
#define DOMID_FIRST_RESERVED (0x7FF0U)

/* DOMID_SELF is used in certain contexts to refer to oneself. */
#define DOMID_SELF (0x7FF0U)

/*
 * DOMID_IO is used to restrict page-table updates to mapping I/O memory.
 * Although no Foreign Domain need be specified to map I/O pages, DOMID_IO
 * is useful to ensure that no mappings to the OS's own heap are accidentally
 * installed. (e.g., in Linux this could cause havoc as reference counts
 * aren't adjusted on the I/O-mapping code path).
 * This only makes sense in MMUEXT_SET_FOREIGNDOM, but in that context can
 * be specified by any calling domain.
 */
#define DOMID_IO   (0x7FF1U)

/*
 * DOMID_XEN is used to allow privileged domains to map restricted parts of
 * Xen's heap space (e.g., the machine_to_phys table).
 * This only makes sense in MMUEXT_SET_FOREIGNDOM, and is only permitted if
 * the caller is privileged.
 */
#define DOMID_XEN  (0x7FF2U)

/*
 * DOMID_COW is used as the owner of sharable pages */
#define DOMID_COW  (0x7FF3U)

/* DOMID_INVALID is used to identify pages with unknown owner. */
#define DOMID_INVALID (0x7FF4U)

/* Idle domain. */
#define DOMID_IDLE (0x7FFFU)

/*
 * Event channel endpoints per domain:
 *  1024 if a long is 32 bits; 4096 if a long is 64 bits.
 */
#define NR_EVENT_CHANNELS (sizeof(unsigned long) * sizeof(unsigned long) * 64)

/* New console union for dom0 introduced in 0x00030203. */
#if __XEN_INTERFACE_VERSION__ < 0x00030203
#define console_mfn    console.domU.mfn
#define console_evtchn console.domU.evtchn
#endif

/* These flags are passed in the 'flags' field of start_info_t. */
#define SIF_PRIVILEGED    (1<<0)  /* Is the domain privileged? */
#define SIF_INITDOMAIN    (1<<1)  /* Is this the initial control domain? */
#define SIF_MULTIBOOT_MOD (1<<2)  /* Is mod_start a multiboot module? */
#define SIF_MOD_START_PFN (1<<3)  /* Is mod_start a PFN? */
#define SIF_PM_MASK       (0xFF<<8) /* reserve 1 byte for xen-pm options */

/*
 * A multiboot module is a package containing modules very similar to a
 * multiboot module array. The only differences are:
 * - the array of module descriptors is by convention simply at the beginning
 *   of the multiboot module,
 * - addresses in the module descriptors are based on the beginning of the
 *   multiboot module,
 * - the number of modules is determined by a termination descriptor that has
 *   mod_start == 0.
 *
 * This permits to both build it statically and reference it in a configuration
 * file, and let the PV guest easily rebase the addresses to virtual addresses
 * and at the same time count the number of modules.
 */
struct xen_multiboot_mod_list
{
    /* Address of first byte of the module */
    uint32_t mod_start;
    /* Address of last byte of the module (inclusive) */
    uint32_t mod_end;
    /* Address of zero-terminated command line */
    uint32_t cmdline;
    /* Unused, must be zero */
    uint32_t pad;
};

typedef struct dom0_vga_console_info {
    uint8_t video_type; /* DOM0_VGA_CONSOLE_??? */
#define XEN_VGATYPE_TEXT_MODE_3 0x03
#define XEN_VGATYPE_VESA_LFB    0x23

    union {
        struct {
            /* Font height, in pixels. */
            uint16_t font_height;
            /* Cursor location (column, row). */
            uint16_t cursor_x, cursor_y;
            /* Number of rows and columns (dimensions in characters). */
            uint16_t rows, columns;
        } text_mode_3;

        struct {
            /* Width and height, in pixels. */
            uint16_t width, height;
            /* Bytes per scan line. */
            uint16_t bytes_per_line;
            /* Bits per pixel. */
            uint16_t bits_per_pixel;
            /* LFB physical address, and size (in units of 64kB). */
            uint32_t lfb_base;
            uint32_t lfb_size;
            /* RGB mask offsets and sizes, as defined by VBE 1.2+ */
            uint8_t  red_pos, red_size;
            uint8_t  green_pos, green_size;
            uint8_t  blue_pos, blue_size;
            uint8_t  rsvd_pos, rsvd_size;
#if __XEN_INTERFACE_VERSION__ >= 0x00030206
            /* VESA capabilities (offset 0xa, VESA command 0x4f00). */
            uint32_t gbl_caps;
            /* Mode attributes (offset 0x0, VESA command 0x4f01). */
            uint16_t mode_attrs;
#endif
        } vesa_lfb;
    } u;
} dom0_vga_console_info_t;
#define xen_vga_console_info dom0_vga_console_info
#define xen_vga_console_info_t dom0_vga_console_info_t

typedef uint8_t xen_domain_handle_t[16];

/* Turn a plain number into a C unsigned long constant. */
#define __mk_unsigned_long(x) x ## UL
#define mk_unsigned_long(x) __mk_unsigned_long(x)

#else /* __ASSEMBLY__ */

/* In assembly code we cannot use C numeric constant suffixes. */
#define mk_unsigned_long(x) x

#endif /* !__ASSEMBLY__ */

/* Default definitions for macros used by domctl/sysctl. */
#if defined(__XEN__) || defined(__XEN_TOOLS__)

#ifndef uint64_aligned_t
#define uint64_aligned_t uint64_t
#endif
#ifndef XEN_GUEST_HANDLE_64
#define XEN_GUEST_HANDLE_64(name) XEN_GUEST_HANDLE(name)
#endif

#ifndef __ASSEMBLY__
struct xenctl_cpumap {
    XEN_GUEST_HANDLE_64(uint8) bitmap;
    uint32_t nr_cpus;
};
#endif

#endif /* defined(__XEN__) || defined(__XEN_TOOLS__) */

#endif /* __XEN_PUBLIC_XEN_H__ */

/*
 * Local variables:
 * mode: C
 * c-set-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */

/*
 * arch-x86/cpuid.h
 */

/******************************************************************************
 * arch-x86/cpuid.h
 * 
 * CPUID interface to Xen.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * 
 * Copyright (c) 2007 Citrix Systems, Inc.
 * 
 * Authors:
 *    Keir Fraser <keir@xen.org>
 */

#ifndef __XEN_PUBLIC_ARCH_X86_CPUID_H__
#define __XEN_PUBLIC_ARCH_X86_CPUID_H__

/* Xen identification leaves start at 0x40000000. */
#define XEN_CPUID_FIRST_LEAF 0x40000000
#define XEN_CPUID_LEAF(i)    (XEN_CPUID_FIRST_LEAF + (i))

/*
 * Leaf 1 (0x40000000)
 * EAX: Largest Xen-information leaf. All leaves up to an including @EAX
 *      are supported by the Xen host.
 * EBX-EDX: "XenVMMXenVMM" signature, allowing positive identification
 *      of a Xen host.
 */
#define XEN_CPUID_SIGNATURE_EBX 0x566e6558 /* "XenV" */
#define XEN_CPUID_SIGNATURE_ECX 0x65584d4d /* "MMXe" */
#define XEN_CPUID_SIGNATURE_EDX 0x4d4d566e /* "nVMM" */

/*
 * Leaf 2 (0x40000001)
 * EAX[31:16]: Xen major version.
 * EAX[15: 0]: Xen minor version.
 * EBX-EDX: Reserved (currently all zeroes).
 */

/*
 * Leaf 3 (0x40000002)
 * EAX: Number of hypercall transfer pages. This register is always guaranteed
 *      to specify one hypercall page.
 * EBX: Base address of Xen-specific MSRs.
 * ECX: Features 1. Unused bits are set to zero.
 * EDX: Features 2. Unused bits are set to zero.
 */

/* Does the host support MMU_PT_UPDATE_PRESERVE_AD for this guest? */
#define _XEN_CPUID_FEAT1_MMU_PT_UPDATE_PRESERVE_AD 0
#define XEN_CPUID_FEAT1_MMU_PT_UPDATE_PRESERVE_AD  (1u<<0)

#endif /* __XEN_PUBLIC_ARCH_X86_CPUID_H__ */

/*
 * hvm/hvm_op.h
 */

/*
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef __XEN_PUBLIC_HVM_HVM_OP_H__
#define __XEN_PUBLIC_HVM_HVM_OP_H__

/* Get/set subcommands: extra argument == pointer to xen_hvm_param struct. */
#define HVMOP_set_param           0
#define HVMOP_get_param           1
struct xen_hvm_param {
    domid_t  domid;    /* IN */
    uint32_t index;    /* IN */
    uint64_t value;    /* IN/OUT */
};
typedef struct xen_hvm_param xen_hvm_param_t;

/* Set the logical level of one of a domain's PCI INTx wires. */
#define HVMOP_set_pci_intx_level  2
struct xen_hvm_set_pci_intx_level {
    /* Domain to be updated. */
    domid_t  domid;
    /* PCI INTx identification in PCI topology (domain:bus:device:intx). */
    uint8_t  domain, bus, device, intx;
    /* Assertion level (0 = unasserted, 1 = asserted). */
    uint8_t  level;
};
typedef struct xen_hvm_set_pci_intx_level xen_hvm_set_pci_intx_level_t;

/* Set the logical level of one of a domain's ISA IRQ wires. */
#define HVMOP_set_isa_irq_level   3
struct xen_hvm_set_isa_irq_level {
    /* Domain to be updated. */
    domid_t  domid;
    /* ISA device identification, by ISA IRQ (0-15). */
    uint8_t  isa_irq;
    /* Assertion level (0 = unasserted, 1 = asserted). */
    uint8_t  level;
};
typedef struct xen_hvm_set_isa_irq_level xen_hvm_set_isa_irq_level_t;

#define HVMOP_set_pci_link_route  4
struct xen_hvm_set_pci_link_route {
    /* Domain to be updated. */
    domid_t  domid;
    /* PCI link identifier (0-3). */
    uint8_t  link;
    /* ISA IRQ (1-15), or 0 (disable link). */
    uint8_t  isa_irq;
};
typedef struct xen_hvm_set_pci_link_route xen_hvm_set_pci_link_route_t;

/* Flushes all VCPU TLBs: @arg must be NULL. */
#define HVMOP_flush_tlbs          5

/* Following tools-only interfaces may change in future. */
#if defined(__XEN__) || defined(__XEN_TOOLS__)

/* Track dirty VRAM. */
#define HVMOP_track_dirty_vram    6
struct xen_hvm_track_dirty_vram {
    /* Domain to be tracked. */
    domid_t  domid;
    /* First pfn to track. */
    uint64_aligned_t first_pfn;
    /* Number of pages to track. */
    uint64_aligned_t nr;
    /* OUT variable. */
    /* Dirty bitmap buffer. */
    XEN_GUEST_HANDLE_64(uint8) dirty_bitmap;
};
typedef struct xen_hvm_track_dirty_vram xen_hvm_track_dirty_vram_t;

/* Notify that some pages got modified by the Device Model. */
#define HVMOP_modified_memory    7
struct xen_hvm_modified_memory {
    /* Domain to be updated. */
    domid_t  domid;
    /* First pfn. */
    uint64_aligned_t first_pfn;
    /* Number of pages. */
    uint64_aligned_t nr;
};
typedef struct xen_hvm_modified_memory xen_hvm_modified_memory_t;

#define HVMOP_set_mem_type    8
typedef enum {
    HVMMEM_ram_rw,             /* Normal read/write guest RAM */
    HVMMEM_ram_ro,             /* Read-only; writes are discarded */
    HVMMEM_mmio_dm,            /* Reads and write go to the device model */
} hvmmem_type_t;
/* Notify that a region of memory is to be treated in a specific way. */
struct xen_hvm_set_mem_type {
    /* Domain to be updated. */
    domid_t domid;
    /* Memory type */
    uint64_aligned_t hvmmem_type;
    /* First pfn. */
    uint64_aligned_t first_pfn;
    /* Number of pages. */
    uint64_aligned_t nr;
};
typedef struct xen_hvm_set_mem_type xen_hvm_set_mem_type_t;

#endif /* defined(__XEN__) || defined(__XEN_TOOLS__) */

/* Hint from PV drivers for pagetable destruction. */
#define HVMOP_pagetable_dying        9
struct xen_hvm_pagetable_dying {
    /* Domain with a pagetable about to be destroyed. */
    domid_t  domid;
    uint16_t pad[3]; /* align next field on 8-byte boundary */
    /* guest physical address of the toplevel pagetable dying */
    uint64_t gpa;
};
typedef struct xen_hvm_pagetable_dying xen_hvm_pagetable_dying_t;

/* Get the current Xen time, in nanoseconds since system boot. */
#define HVMOP_get_time              10
struct xen_hvm_get_time {
    uint64_t now;      /* OUT */
};
typedef struct xen_hvm_get_time xen_hvm_get_time_t;

#define HVMOP_set_mem_access        12
typedef enum {
    HVMMEM_access_n,
    HVMMEM_access_r,
    HVMMEM_access_w,
    HVMMEM_access_rw,
    HVMMEM_access_x,
    HVMMEM_access_rx,
    HVMMEM_access_wx,
    HVMMEM_access_rwx,
    HVMMEM_access_rx2rw,       /* Page starts off as read-execute, but automatically change
				* to read-write on a write */
    HVMMEM_access_default      /* Take the domain default */
} hvmmem_access_t;
/* Notify that a region of memory is to have specific access types */
struct xen_hvm_set_mem_access {
    /* Domain to be updated. */
    domid_t domid;
    uint16_t pad[3]; /* align next field on 8-byte boundary */
    /* Memory type */
    uint64_t hvmmem_access; /* hvm_access_t */
    /* First pfn, or ~0ull to set the default access for new pages */
    uint64_t first_pfn;
    /* Number of pages, ignored on setting default access */
    uint64_t nr;
};
typedef struct xen_hvm_set_mem_access xen_hvm_set_mem_access_t;

#define HVMOP_get_mem_access        13
/* Get the specific access type for that region of memory */
struct xen_hvm_get_mem_access {
    /* Domain to be queried. */
    domid_t domid;
    uint16_t pad[3]; /* align next field on 8-byte boundary */
    /* Memory type: OUT */
    uint64_t hvmmem_access; /* hvm_access_t */
    /* pfn, or ~0ull for default access for new pages.  IN */
    uint64_t pfn;
};
typedef struct xen_hvm_get_mem_access xen_hvm_get_mem_access_t;

#define HVMOP_inject_trap            14
/* Inject a trap into a VCPU, which will get taken up on the next
 * scheduling of it. Note that the caller should know enough of the
 * state of the CPU before injecting, to know what the effect of
 * injecting the trap will be.
 */
struct xen_hvm_inject_trap {
    /* Domain to be queried. */
    domid_t domid;
    /* VCPU */
    uint32_t vcpuid;
    /* Trap number */
    uint32_t trap;
    /* Error code, or -1 to skip */
    uint32_t error_code;
    /* CR2 for page faults */
    uint64_t cr2;
};
typedef struct xen_hvm_inject_trap xen_hvm_inject_trap_t;

#endif /* __XEN_PUBLIC_HVM_HVM_OP_H__ */

/*
 * hvm/params.h
 */

/*
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef __XEN_PUBLIC_HVM_PARAMS_H__
#define __XEN_PUBLIC_HVM_PARAMS_H__

/*
 * Parameter space for HVMOP_{set,get}_param.
 */

/*
 * How should CPU0 event-channel notifications be delivered?
 * val[63:56] == 0: val[55:0] is a delivery GSI (Global System Interrupt).
 * val[63:56] == 1: val[55:0] is a delivery PCI INTx line, as follows:
 *                  Domain = val[47:32], Bus  = val[31:16],
 *                  DevFn  = val[15: 8], IntX = val[ 1: 0]
 * val[63:56] == 2: val[7:0] is a vector number, check for
 *                  XENFEAT_hvm_callback_vector to know if this delivery
 *                  method is available.
 * If val == 0 then CPU0 event-channel notifications are not delivered.
 */
#define HVM_PARAM_CALLBACK_IRQ 0

/*
 * These are not used by Xen. They are here for convenience of HVM-guest
 * xenbus implementations.
 */
#define HVM_PARAM_STORE_PFN    1
#define HVM_PARAM_STORE_EVTCHN 2

#define HVM_PARAM_PAE_ENABLED  4

#define HVM_PARAM_IOREQ_PFN    5

#define HVM_PARAM_BUFIOREQ_PFN 6

#ifdef __ia64__

#define HVM_PARAM_NVRAM_FD     7
#define HVM_PARAM_VHPT_SIZE    8
#define HVM_PARAM_BUFPIOREQ_PFN	9

#elif defined(__i386__) || defined(__x86_64__)

/* Expose Viridian interfaces to this HVM guest? */
#define HVM_PARAM_VIRIDIAN     9

#endif

/*
 * Set mode for virtual timers (currently x86 only):
 *  delay_for_missed_ticks (default):
 *   Do not advance a vcpu's time beyond the correct delivery time for
 *   interrupts that have been missed due to preemption. Deliver missed
 *   interrupts when the vcpu is rescheduled and advance the vcpu's virtual
 *   time stepwise for each one.
 *  no_delay_for_missed_ticks:
 *   As above, missed interrupts are delivered, but guest time always tracks
 *   wallclock (i.e., real) time while doing so.
 *  no_missed_ticks_pending:
 *   No missed interrupts are held pending. Instead, to ensure ticks are
 *   delivered at some non-zero rate, if we detect missed ticks then the
 *   internal tick alarm is not disabled if the VCPU is preempted during the
 *   next tick period.
 *  one_missed_tick_pending:
 *   Missed interrupts are collapsed together and delivered as one 'late tick'.
 *   Guest time always tracks wallclock (i.e., real) time.
 */
#define HVM_PARAM_TIMER_MODE   10
#define HVMPTM_delay_for_missed_ticks    0
#define HVMPTM_no_delay_for_missed_ticks 1
#define HVMPTM_no_missed_ticks_pending   2
#define HVMPTM_one_missed_tick_pending   3

/* Boolean: Enable virtual HPET (high-precision event timer)? (x86-only) */
#define HVM_PARAM_HPET_ENABLED 11

/* Identity-map page directory used by Intel EPT when CR0.PG=0. */
#define HVM_PARAM_IDENT_PT     12

/* Device Model domain, defaults to 0. */
#define HVM_PARAM_DM_DOMAIN    13

/* ACPI S state: currently support S0 and S3 on x86. */
#define HVM_PARAM_ACPI_S_STATE 14

/* TSS used on Intel when CR0.PE=0. */
#define HVM_PARAM_VM86_TSS     15

/* Boolean: Enable aligning all periodic vpts to reduce interrupts */
#define HVM_PARAM_VPT_ALIGN    16

/* Console debug shared memory ring and event channel */
#define HVM_PARAM_CONSOLE_PFN    17
#define HVM_PARAM_CONSOLE_EVTCHN 18

/*
 * Select location of ACPI PM1a and TMR control blocks. Currently two locations
 * are supported, specified by version 0 or 1 in this parameter:
 *   - 0: default, use the old addresses
 *        PM1A_EVT == 0x1f40; PM1A_CNT == 0x1f44; PM_TMR == 0x1f48
 *   - 1: use the new default qemu addresses
 *        PM1A_EVT == 0xb000; PM1A_CNT == 0xb004; PM_TMR == 0xb008
 * You can find these address definitions in <hvm/ioreq.h>
 */
#define HVM_PARAM_ACPI_IOPORTS_LOCATION 19

/* Enable blocking memory events, async or sync (pause vcpu until response) 
 * onchangeonly indicates messages only on a change of value */
#define HVM_PARAM_MEMORY_EVENT_CR0   20
#define HVM_PARAM_MEMORY_EVENT_CR3   21
#define HVM_PARAM_MEMORY_EVENT_CR4   22
#define HVM_PARAM_MEMORY_EVENT_INT3  23

#define HVMPME_MODE_MASK       (3 << 0)
#define HVMPME_mode_disabled   0
#define HVMPME_mode_async      1
#define HVMPME_mode_sync       2
#define HVMPME_onchangeonly    (1 << 2)

#define HVM_NR_PARAMS          24

#endif /* __XEN_PUBLIC_HVM_PARAMS_H__ */

#if __amd64__
#include <dev/xen/hypercall_amd64.h>
#elif __i386__
#include <dev/xen/hypercall_i386.h>
#else
#error Unsupported architecture
#endif

#endif	/* !_DEV_XEN_HYPERVISOR_H_ */
