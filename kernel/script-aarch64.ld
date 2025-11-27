/* kernel/script-aarch64.ld */

ENTRY(_start)

/* QEMU aarch64 virt loads the kernel at 0x40000000 */
KERNEL_LOAD_ADDR = 0x40000000;

SECTIONS
{
    . = KERNEL_LOAD_ADDR;

    .text.boot : {
        KEEP(*(.text.boot))
    }

    .text : {
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    . = ALIGN(16);
    __bss_start = .;
    .bss : {
        *(.bss*)
        *(COMMON)
    }
    __bss_end = .;
}
