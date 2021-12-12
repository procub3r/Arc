#ifndef IO_H
#define IO_H

#include "../boot/stivale2.h"
#include <stdint.h>

void InitIO(struct stivale2_struct* stivale2_struct);
void print(const char* string);
void print_hex(uint64_t number);

#endif // IO_H
