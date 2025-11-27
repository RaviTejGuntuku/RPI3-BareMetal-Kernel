// kernel/start.S

    .section .text.boot, "ax"
    .align  7
    .global _start

_start:
    // Only let core 0 run; others park in wfe
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF
    cbz     x0, 1f

0:  wfe
    b       0b

1:
    // Set up a simple stack
    adrp    x0, stack_top
    add     x0, x0, :lo12:stack_top
    mov     sp, x0

    // Clear .bss
    adrp    x1, __bss_start
    add     x1, x1, :lo12:__bss_start

    adrp    x2, __bss_end
    add     x2, x2, :lo12:__bss_end

2:  cmp     x1, x2
    b.ge    3f
    str     xzr, [x1], #8
    b       2b

3:
    // Call C entry
    bl      kernelInit

    // If it returns, just park
4:  wfe
    b       4b

    .section .bss, "aw", %nobits
    .align  16
stack:
    .skip   16384         // 16KB stack
stack_top:
