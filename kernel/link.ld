SECTIONS
{
    . = 0x80000;
    .text : { KEEP(*(.text.boot)) *(.text .text.* .gnu.linkonce.t*) }
    .rodata : { *(.rodata .rodata.* .gnu.linkonce.r*) }
    .init_array :
    {
        /* first, your sentinel start from crti.o */
        build/crti.o(.init_array)

        /* all other constructors */
        KEEP (*(SORT(EXCLUDE_FILE(build/crti.o build/crtn.o) .init_array*)))
        KEEP (*(EXCLUDE_FILE(build/crti.o build/crtn.o) .init_array))

        /* sentinel end from crtn.o */
        build/crtn.o(.init_array)
    }
    .fini_array :
    {
        build/crti.o(.fini_array)
        KEEP (*(SORT(EXCLUDE_FILE(build/crti.o build/crtn.o) .fini_array*)))
        KEEP (*(EXCLUDE_FILE(build/crti.o build/crtn.o) .fini_array))
        build/crtn.o(.fini_array)
    }
    PROVIDE(_data = .);
    .data : { *(.data .data.* .gnu.linkonce.d*) }
    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss.bootstack)     /* include boot stack in BSS */
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
    }
    _end = .;

   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}
__bss_size = (__bss_end - __bss_start)>>3;