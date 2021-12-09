#include <stddef.h>
#include "stivale2.h"
#include "boot.h"

// Stivale2 terminal:
static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        .next = 0
    },
    .flags = 0
};

// Stivale2 framebuffer:
static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = (uint64_t)&terminal_hdr_tag
    },
    
    // Set framebuffer attributes to 0 to let
    // the bootloader pick appropriate values:
    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp = 0
};

// Allocate the stack as an array in the .bss section:
// Align it to 16 bytes as required by the stivale2 spec.
static uint8_t stack[4096] __attribute__((aligned(16)));

// Stivale2 header in .stivale2hdr section:
__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale2_header = {
    // Set entry_point to zero to jump to the
    // entry point configured by the linker:
    .entry_point = 0,

    // Set stack to the end of our stack array
    // because the stack grows downwards:
    .stack = (uint64_t)stack + sizeof(stack),

    // Set flags: (Refer to STIVALE2.md for spec)
    .flags = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),

    // Pointer to the first tag in a linked list of tags:
    .tags = (uint64_t)&framebuffer_hdr_tag
};

// Return stivale2 struct tag of requested id:
void* stivale2_get_tag(struct stivale2_struct* stivale2_struct, uint64_t id) {
    struct stivale2_tag* current_tag = (void*)stivale2_struct->tags;

    // Traverse linked list of struct tags:
    for (;;) {
        // Return NULL if search failed:
        if (current_tag == NULL) {
            return NULL;
        }

        // Return pointer to current_tag if id matches:
        if (current_tag->identifier == id) {
            return current_tag;
        }

        // Set current_tag to next element in list:
        current_tag = (void*)current_tag->next;
    }
}
