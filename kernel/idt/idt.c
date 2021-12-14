#include <stdlib.h>
#include "../io/io.h"
#include "idt.h"

// Interrupt names:
static const char* interrupt_names[32] = {
    "Divide By Zero",
    "Debug Exception",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 FPU Floating Point Error",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};

// Located in idt.asm
extern uint64_t isr_pointers[];
extern void load_idt(struct IdtRegister* idtr);

// Interrupt Descriptor Table, aligned to an
// 8 byte boundary for cache optimization:
static struct IdtEntry idt[256] __attribute__((aligned(8)));

// IDT Register:
static struct IdtRegister idtr = {
    .limit = sizeof(idt) - 1,
    .base_addr = (uint64_t)&idt
};

static void SetIdtEntry(uint64_t vector, struct IdtEntry* entry, uint8_t type_attr) {
    entry->offset_low = isr_pointers[vector] & 0xFFFF;
    entry->segment_selector = 0x28; // Stivale2 bootloader sets CS to 0x28
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
    for (uint64_t i = 0; i < 32; i++) {
        // 0x8E = type attribute of an interrupt gate.
        SetIdtEntry(i, &idt[i], 0x8E);
    }
    print("Done\n");

    // Load IDT:
    print("[KERNEL]: Loading IDT... ");
    load_idt(&idtr); // asm call
    print("Done\n");
}

void interrupt_handler(struct IsrParams* isr_params) {
    print("[Interrupt Handler]: Interrupt triggered:\n");
    printf("\tVector: %x2\n", isr_params->interrupt_vector);
    printf("\tInterrupt: %s\n", interrupt_names[isr_params->interrupt_vector]);
}
