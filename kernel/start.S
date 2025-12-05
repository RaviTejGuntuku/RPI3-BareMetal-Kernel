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

// ---------------------------------------------------------------------
// Temporary boot stacks (one shared region).
// This is only used *before* we switch to the per-CPU stacks.
// ---------------------------------------------------------------------
    .section ".bss.bootstack","aw",%nobits
    .align 16
boot_stack:
    .skip 4096                // 4 KB boot stack
boot_stack_top:               // label points just past the stack

    .section ".text.boot"
    .align 4

_start:
    //------------------------------------------------------------------
    // Determine core ID from MPIDR_EL1
    // On Raspberry Pi 3: Aff0 (bits [1:0]) is the core number 0..3
    //------------------------------------------------------------------
    mrs     x0, mpidr_el1
    and     x0, x0, #0x3        // x0 = core_id (0,1,2,3)

    //------------------------------------------------------------------
    // Only core 0 clears the .bss section
    //------------------------------------------------------------------
    cbnz    x0, 1f              // if core_id != 0, skip BSS clear

    ldr     x1, =__bss_start    // x1 = start of .bss
    ldr     x2, =__bss_end      // x2 = end of .bss
0:  cmp     x1, x2
    b.hs    1f                  // if x1 >= x2, done
    str     xzr, [x1], #8       // store zero and advance
    b       0b

1:
    //------------------------------------------------------------------
    // Install a temporary boot stack so we can safely call C/C++.
    // This is similar to x86 using tempStack before pickKernelStack().
    //------------------------------------------------------------------
    ldr     x3, =boot_stack_top
    mov     sp, x3              // SP = top of temp boot stack

    //------------------------------------------------------------------
    // Ask C++ for our per-CPU stack:
    //   extern "C" uint64_t pickKernelStack(void);
    // return (x0) becomes our new SP.
    //------------------------------------------------------------------
    bl      pickKernelStack     // x0 = per-CPU stack pointer
    mov     sp, x0              // install per-CPU stack

    //------------------------------------------------------------------
    // Now jump into C++ kernelInit().
    //------------------------------------------------------------------
    bl      kernelInit

    //------------------------------------------------------------------
    // If kernelInit returns, just park this core.
    //------------------------------------------------------------------
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
