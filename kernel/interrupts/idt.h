#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// The struct of an entry in the
// IDT (Interrupt Descriptor Table):
struct IdtEntry {
    uint16_t offset_low;
    uint16_t segment_selector;
    uint8_t ist; // Interrupt Stack Table
    uint8_t type_attributes;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed));

// IDTR struct:
struct IdtRegister {
    uint16_t limit;
    uint64_t base_addr;
} __attribute__((packed));

#endif // IDT_H
