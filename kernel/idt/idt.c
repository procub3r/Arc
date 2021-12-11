#include "idt.h"

// Interrupt Descriptor Table, aligned to an
// 8 byte boundary for cache optimization:
static struct IdtEntry idt[256] __attribute__((aligned(8)));

// IDT Register:
static struct IdtRegister idtr = {
    .limit = sizeof(idt) - 1,
    .base_addr = (uint64_t)&idt
};

// Initialize IDT:
void InitIdt() {
    load_idt(idtr);
}
