#ifndef IO_H
#define IO_H

#include "../boot/stivale2.h"
#include <stdint.h>

void InitIO(struct stivale2_struct* stivale2_struct);
void print(const char* string);
void printf(const char* string, ...);
void print_hex2(uint16_t number);
void print_hex4(uint32_t number);
void print_hex8(uint64_t number);

#endif // IO_H
