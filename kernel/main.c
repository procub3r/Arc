#include <stdint.h>
#include <stddef.h>
#include "boot/stivale2.h"
#include "boot/boot.h"
#include "io/io.h"

// Kernel's entry point:
void kernel_main(struct stivale2_struct* stivale2_struct) {
    // Initialize IO:
    InitIO(stivale2_struct);

    // Hang indefinitely:
    for (;;) {
        asm("hlt");
    }
}
