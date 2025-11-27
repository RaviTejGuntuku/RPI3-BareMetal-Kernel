// machine.S

    .section .rodata
    .align  3
exit_block:
    .word   0x20026      // ADP_Stopped_ApplicationExit
    .word   0            // exit status = 0

    .section .text
    .global qemu_exit
qemu_exit:
    // x0 = SYS_EXIT (0x18)
    // x1 = pointer to [reason, status]
    adrp    x1, exit_block
    add     x1, x1, :lo12:exit_block
    mov     w0, #0x18
    hlt     #0xF000

1:  wfe
    b       1b