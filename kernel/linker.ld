/* Generate an ELF64 binary for the x86_64 architecture */
OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

/* Set symbol `kernel_main` as the entry point */
ENTRY(kernel_main)

/* Define program headers to get the right */
/* MMU permissions from the bootloader */
PHDRS {
    null    PT_NULL    FLAGS(0) ;                   /* Null segment */
    text    PT_LOAD    FLAGS((1 << 0) | (1 << 2)) ; /* Execute + Read */
    rodata  PT_LOAD    FLAGS((1 << 2)) ;            /* Read only */
    data    PT_LOAD    FLAGS((1 << 1) | (1 << 2)) ; /* Write + Read */
}

/* Determine where all the sections go */
SECTIONS {
    . = 0xffffffff80000000;
    .text : {
        *(.text*)
    } :text

    /* Move to the next memory page for .stivale2hdr and .rodata */
    . += CONSTANT(MAXPAGESIZE);

    /* We place the .stivale2hdr section containing the header in its own section, */
    /* and we use the KEEP directive on it to make sure it doesn't get discarded. */
    .stivale2hdr : {
        KEEP(*(.stivale2hdr))
    } :rodata

    .rodata : {
        *(.rodata*)
    } :rodata

    /* Move to the next memory page for .data and .bss */
    . += CONSTANT(MAXPAGESIZE);

    .data : {
        *(.data*)
    } :data

    .bss : {
        *(COMMON)
        *(.bss*)
    } :data
}
