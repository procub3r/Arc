#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
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

    print("[KERNEL]: Initialized IO. Welcome to Arc!\n");
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

void printf(const char* string, ...) {
    va_list argp;
    va_start(argp, string);

    for (size_t i = 0; i < strlen(string); i++) {
        if (string[i] == '%') {
            switch (string[++i]) {
                case 'x': {
                    switch (string[++i]) {
                        case '2': {
                            uint16_t number = va_arg(argp, int);
                            print_hex2(number);
                            break;
                        }
                        case '4': {
                            uint32_t number = va_arg(argp, int);
                            print_hex4(number);
                            break;
                        }
                        case '8': {
                            uint64_t number = va_arg(argp, uint64_t);
                            print_hex8(number);
                            break;
                        }
                    }
                    break;
                }
                case 's': {
                    const char* string = va_arg(argp, const char*);
                    print(string);
                    break;
                }
                case 'c': {
                    char character = va_arg(argp, int);
                    term_write(&character, 1);
                    break;
                }
                case '%': {
                    term_write(&string[i], 1);
                    break;
                }
            }
        } else {
            term_write(&string[i], 1);
        }
    }

    va_end(argp);
}

void print_hex2(uint16_t number) {
    char result[] = "0x0000";
    uint8_t index = 5;
    while (number != 0) {
        uint8_t digit = (uint8_t)number & 0x0F;
        result[index] = digit + (digit < 10 ? 48 : 55);
        number >>= 4;
        index--;
    }
    print(result);
}

void print_hex4(uint32_t number) {
    char result[] = "0x00000000";
    uint8_t index = 9;
    while (number != 0) {
        uint8_t digit = (uint8_t)number & 0x0F;
        result[index] = digit + (digit < 10 ? 48 : 55);
        number >>= 4;
        index--;
    }
    print(result);
}

void print_hex8(uint64_t number) {
    char result[] = "0x0000000000000000";
    uint8_t index = 17;
    while (number != 0) {
        uint8_t digit = (uint8_t)number & 0x0F;
        result[index] = digit + (digit < 10 ? 48 : 55);
        number >>= 4;
        index--;
    }
    print(result);
}
