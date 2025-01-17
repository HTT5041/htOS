#include <idt.h>
#include <logging.h>

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, uint32_t handler){
    idt[n].low_offset = (uint16_t) (handler) & 0xFFFF;
    idt[n].selector = 0x08;
    idt[n].zero = 0;
    idt[n].flags = 0x8E;
    idt[n].high_offset = (uint16_t) (handler >> 16) & 0xFFFF;
}


void set_idt(){
    idt_reg.base = (uint32_t) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;


    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}

