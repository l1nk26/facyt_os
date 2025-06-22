ENTRY(_start)

PHDRS {
    text PT_LOAD FLAGS((1 << 0) | (1 << 2));  /* RX (Read + Execute) */
    rodata PT_LOAD FLAGS(1 << 0);             /* R (Read-only) */
    data PT_LOAD FLAGS((1 << 0) | (1 << 1));  /* RW (Read + Write) */
    bss PT_LOAD FLAGS((1 << 0) | (1 << 1));   /* RW */
}

SECTIONS
{
    /* El kernel comienza en la dirección 1MB */
    . = 1M; 

    .text : {
        *(.multiboot_header) 
        *(.text*)
    } :text

    .rodata : ALIGN(4K) {
        *(.rodata*)
    } :rodata

    .data : ALIGN(4K) {
        *(.data*)
    } :data

    .bss : ALIGN(4K) {
        *(.bss*)
        *(COMMON)
    } :bss

    /* Ensure the binary has a reasonable size */
    /DISCARD/ : {
        *(.note*)
        *(.comment*)
    }
}