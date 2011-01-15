/*	$OpenBSD$	*/

/*
 * This is a compilation of only necessary excerpts from the
 * Xen public headers which is enough to build paravirtual
 * drivers. Original copyright notices are carefully merged.
 */

/*
 * Copyright (c) 2004 K A Fraser
 * Copyright (c) 2007 Citrix Systems, Inc.
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
 */

#ifndef _DEV_XEN_HYPERVISOR_H_
#define _DEV_XEN_HYPERVISOR_H_

/* arch-x86/cpuid.h */

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


/* xen.h */

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

#if __amd64__
#include <dev/xen/hypercall_amd64.h>
#elif __i386__
#include <dev/xen/hypercall_i386.h>
#else
#error Unsupported architecture
#endif

#endif	/* !_DEV_XEN_HYPERVISOR_H_ */
