/*
 * AArch64 bootstrap for Raspberry Pi 3 / QEMU raspi3b
 *
 *  - Core 0 clears .bss
 *  - All cores first use a small temporary boot stack
 *  - Then C++ gives each core its real per-CPU stack via pickKernelStack()
 *  - Then each core calls kernelInit()
 */

    .section ".text.boot"
    .align  4

    .global _start
    .global secondary_entry

    .extern kernelInit
    .extern pickKernelStack
    .extern __bss_start
    .extern __bss_end
    .section ".bss.bootstack","aw",%nobits
    .align 16
boot_stack:
    .skip 4096
boot_stack_top:

    .section ".text.boot"
    .align 4
_start:
    // Determine core ID from mpidr_el1
    mrs     x0, mpidr_el1
    and     x0, x0, #0x3
    // Only core 0 clears the .bss section
    cbnz    x0, 1f
    ldr     x1, =__bss_start    // x1 = start of .bss
    ldr     x2, =__bss_end      // x2 = end of .bss
0:  cmp     x1, x2
    b.hs    1f
    str     xzr, [x1], #8
    b       0b
1:
    ldr     x3, =boot_stack_top
    mov     sp, x3              // SP = top of temp boot stack
    bl      pickKernelStack     // x0 = per-CPU stack pointer
    mov     sp, x0              // install per-CPU stack
    bl      kernelInit
2:
    wfe
    b       2b

// ---------------------------------------------------------------------
// Secondary entry for PSCI cpu_on()
// SMP::startOthers() uses &secondary_entry as the target address.
// It just funnels secondaries through the same _start path.
// ---------------------------------------------------------------------
secondary_entry:
    b       _start
