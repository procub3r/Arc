#include "../io/io.h"
#include "idt.h"

// Located in idt.asm
extern uint64_t* isr_pointers;
extern void load_idt(struct IdtRegister idtr);

// Interrupt Descriptor Table, aligned to an
// 8 byte boundary for cache optimization:
static struct IdtEntry idt[256] __attribute__((aligned(8)));

// IDT Register:
static struct IdtRegister idtr = {
    .limit = sizeof(idt) - 1,
    .base_addr = (uint64_t)&idt
};

static void SetIdtEntry(int vector, struct IdtEntry* entry, uint8_t type_attr) {
    entry->offset_low = isr_pointers[vector] & 0xFFFF;
    entry->segment_selector = 0x08;
    entry->ist = 0;
    entry->type_attributes = type_attr;
    entry->offset_mid = (isr_pointers[vector] >> 16) & 0xFFFF;
    entry->offset_high = (isr_pointers[vector] >> 32) & 0xFFFFFFFF;
    entry->reserved = 0;
}

// Initialize IDT:
void InitIdt() {
    // Install ISRs:
    print("[KERNEL]: Installing ISRs... ");

    // Set entries in the IDT:
    for (int i = 0; i < 32; i++) {
        // 0x8E = type attribute of an interrupt gate.
        SetIdtEntry(i, &idt[i], 0x8E);
    }
    print("Done\n");

    // Load IDT:
    print("[KERNEL]: Loading IDT... ");
    load_idt(idtr); // asm call
    print("Done\n");
}

void interrupt_handler(struct IsrParams isr_params) {
    print("[Interrupt Handler]: Interrupt handled!\n");
}
