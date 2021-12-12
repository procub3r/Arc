#include <stdint.h>
#include <stddef.h>
#include "boot/boot.h"
#include "io/io.h"
#include "idt/idt.h"

// Kernel's entry point:
void kernel_main(struct stivale2_struct* stivale2_struct) {
    InitIO(stivale2_struct);
    InitIdt();

    // Trigger int 0 for demo:
    asm("int 0");

    // Hang indefinitely:
    for (;;) {
        asm("hlt");
    }
}
