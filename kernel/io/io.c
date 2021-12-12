#include <stdlib.h>
#include <stdint.h>
#include "../boot/boot.h"
#include "io.h"

static struct stivale2_struct_tag_terminal* term_str_tag;
static void (*term_write)(const char* string, size_t length);

void InitIO(struct stivale2_struct* stivale2_struct) {
    // Get terminal struct tag:
    term_str_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID);

    // Hang if terminal struct tag is NULL:
    if (term_str_tag == NULL) {
        for (;;) {
            asm("hlt");
        }
    }

    // Get terminal write function:
    void* term_write_ptr = (void*)term_str_tag->term_write;
    term_write = term_write_ptr;

    print("[KERNEL]: Initialized IO.\n");
}

static size_t strlen(const char* string) {
    size_t length = 0;
    while (string[length] != '\0') {
        length++;
    }
    return length;
}

void print(const char* string) {
    term_write(string, strlen(string));
}

void print_hex(uint64_t number) {
    char result[] = "0x0000000000000000";
    int index = 17;
    while (number != 0) {
        int digit = number & 0xF;
        if (digit < 10) {
            result[index] = digit + 48;
        } else {
            result[index] = digit + 55;
        }
        number >>= 4;
        index--;
    }
    print(result);
}
