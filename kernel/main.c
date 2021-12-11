#include <stdint.h>
#include <stddef.h>
#include "boot/stivale2.h"
#include "boot/boot.h"
#include "idt/idt.h"

// Kernel's entry point:
void kernel_main(struct stivale2_struct* stivale2_struct) {
    // Get terminal struct tag:
    struct stivale2_struct_tag_terminal* term_str_tag;
    term_str_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID);

    // Hang if terminal struct tag is NULL:
    if (term_str_tag == NULL) {
        for (;;) {
            asm("hlt");
        }
    }

    // Get terminal write function:
    void* term_write_ptr = (void*)term_str_tag->term_write;
    void (*term_write)(const char* string, size_t length) = term_write_ptr;

    // Print!
    term_write("Hello user. We have been expecting you...", 41);

    // Initialize Interrupts:
    InitIdt();

    // Hang after printing:
    for (;;) {
        asm("hlt");
    }
}
