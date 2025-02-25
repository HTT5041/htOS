#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef struct {
    uint16_t low_offset; // Lower 16 bits of handler address
    uint16_t selector;   // Kernel segemnt selector 
    uint8_t zero;
    /* First byte
     * Bit 7: "Interrupt is present"
     * Bits 6-5: Privilege level of caller (0=kernel..3=user)
     * Bit 4: Set to 0 for interrupt gates
     * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */
    uint8_t flags;
    uint16_t high_offset; // Higher 16 bits of handler address
} __attribute__((packed)) idt_gate_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;

void set_idt_gate(int n, uint32_t handler);
void set_idt();

#endif
