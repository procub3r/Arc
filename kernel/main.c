#include <stdint.h>
#include <stddef.h>
#include "boot/boot.h"
#include "io/io.h"
#include "idt/idt.h"

// Kernel's entry point:
void kernel_main(struct stivale2_struct* stivale2_struct) {
    InitIO(stivale2_struct);
    InitIdt();

    // Hang indefinitely:
    for (;;) {
        asm("hlt");
    }
}
