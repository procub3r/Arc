#ifndef BOOT_H
#define BOOT_H

#include <stdint.h>
#include "stivale2.h"

// Return stivale2 struct tag of requested id:
void* stivale2_get_tag(struct stivale2_struct* stivale2_struct, uint64_t id);

#endif // BOOT_H
