#include <idt.h>
#include <isr.h>
#include <vga.h>
#include <logging.h>
#include <util.h>
#include <abort.h>
#include <ports.h>

// PIC 
#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

isr_t interrupt_handlers[256];

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
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
    "Reserved",
    "Reserved",
    "Reserved"
};


void isr_install() {
    set_idt_gate(0, (uint32_t)isr0);
    set_idt_gate(1, (uint32_t)isr1);
    set_idt_gate(2, (uint32_t)isr2);
    set_idt_gate(3, (uint32_t)isr3);
    set_idt_gate(4, (uint32_t)isr4);
    set_idt_gate(5, (uint32_t)isr5);
    set_idt_gate(6, (uint32_t)isr6);
    set_idt_gate(7, (uint32_t)isr7);
    set_idt_gate(8, (uint32_t)isr8);
    set_idt_gate(9, (uint32_t)isr9);
    set_idt_gate(10, (uint32_t)isr10);
    set_idt_gate(11, (uint32_t)isr11);
    set_idt_gate(12, (uint32_t)isr12);
    set_idt_gate(13, (uint32_t)isr13);
    set_idt_gate(14, (uint32_t)isr14);
    set_idt_gate(15, (uint32_t)isr15);
    set_idt_gate(16, (uint32_t)isr16);
    set_idt_gate(17, (uint32_t)isr17);
    set_idt_gate(18, (uint32_t)isr18);
    set_idt_gate(19, (uint32_t)isr19);
    set_idt_gate(20, (uint32_t)isr20);
    set_idt_gate(21, (uint32_t)isr21);
    set_idt_gate(22, (uint32_t)isr22);
    set_idt_gate(23, (uint32_t)isr23);
    set_idt_gate(24, (uint32_t)isr24);
    set_idt_gate(25, (uint32_t)isr25);
    set_idt_gate(26, (uint32_t)isr26);
    set_idt_gate(27, (uint32_t)isr27);
    set_idt_gate(28, (uint32_t)isr28);
    set_idt_gate(29, (uint32_t)isr29);
    set_idt_gate(30, (uint32_t)isr30);
    set_idt_gate(31, (uint32_t)isr31);

    //Remap the PIC
    log_message(INFO, "Remapping the PIC", true);
    uint8_t mask1, mask2;

    mask1 = b_in(PIC1_DATA);
	mask2 = b_in(PIC2_DATA);

    b_out(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // Start init sequence in cascade mode
    b_out(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    b_out(PIC1_DATA, 0x20); // ICW2: Master PIC vector offset
	b_out(PIC2_DATA, 0x28); // ICW2: Slave PIC vector offset

    b_out(PIC1_DATA, 4); // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	b_out(PIC2_DATA, 2); // ICW3: tell Slave PIC its cascade identity (0000 0010)

    b_out(PIC1_DATA, ICW4_8086); // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	b_out(PIC2_DATA, ICW4_8086);

    b_out(PIC1_DATA, mask1); // Restore the saved masks
	b_out(PIC2_DATA, mask2);
    
    //b_out(0xA1, 0xFF);
	//b_out(0x21, 0xFF);

    // Set the IRQ gates
    set_idt_gate(32, (uint32_t)irq0);
    set_idt_gate(33, (uint32_t)irq1);
    set_idt_gate(34, (uint32_t)irq2);
    set_idt_gate(35, (uint32_t)irq3);
    set_idt_gate(36, (uint32_t)irq4);
    set_idt_gate(37, (uint32_t)irq5);
    set_idt_gate(38, (uint32_t)irq6);
    set_idt_gate(39, (uint32_t)irq7);
    set_idt_gate(40, (uint32_t)irq8);
    set_idt_gate(41, (uint32_t)irq9);
    set_idt_gate(42, (uint32_t)irq10);
    set_idt_gate(43, (uint32_t)irq11);
    set_idt_gate(44, (uint32_t)irq12);
    set_idt_gate(45, (uint32_t)irq13);
    set_idt_gate(46, (uint32_t)irq14);
    set_idt_gate(47, (uint32_t)irq15);
    
    set_idt();
}

void isr_handler(registers_t r) {
    char buffer[9];

    log_message(ERROR, "Interrupt Recieved: ", false);
    kprint(exception_messages[r.int_no]);
    kprint(" | Error code: 0x");
    uint32_to_hex(r.err_code, buffer);
    kprint(buffer);
    
    kprint("\nRegister dump:\n");

    // DS
    kprint(" DS: ");
    uint32_to_hex(r.ds, buffer);
    kprint(buffer);
    kprint("    ");

    // EDI
    kprint("EDI: ");
    uint32_to_hex(r.edi, buffer);
    kprint(buffer);
    kprint("    ");

    // ESI
    kprint("ESI: ");
    uint32_to_hex(r.esi, buffer);
    kprint(buffer);
    kprint("\n");

    // EBP
    kprint("EBP: ");
    uint32_to_hex(r.ebp, buffer);
    kprint(buffer);
    kprint("    ");

    // ESP
    kprint("ESP: ");
    uint32_to_hex(r.esp, buffer);
    kprint(buffer);
    kprint("    ");

    // EBX
    kprint("EBX: ");
    uint32_to_hex(r.ebx, buffer);
    kprint(buffer);
    kprint("\n");

    // EDX
    kprint("EDX: ");
    uint32_to_hex(r.edx, buffer);
    kprint(buffer);
    kprint("    ");

    // ECX
    kprint("ECX: ");
    uint32_to_hex(r.ecx, buffer);
    kprint(buffer);
    kprint("    ");

    // EAX
    kprint("EAX: ");
    uint32_to_hex(r.eax, buffer);
    kprint(buffer);
    kprint("\n\n");

    // EIP
    kprint("EIP: ");
    uint32_to_hex(r.eip, buffer);
    kprint(buffer);
    kprint("\n");

    // CS
    kprint("CS: ");
    uint32_to_hex(r.cs, buffer);
    kprint(buffer);
    kprint("\n");

    // EFLAGS
    kprint("EFLAGS ");
    uint32_to_hex(r.eflags, buffer);
    kprint(buffer);
    kprint("\n");

    // USERESP
    kprint("USERESP: ");
    uint32_to_hex(r.useresp, buffer);
    kprint(buffer);
    kprint("\n");

    // SS
    kprint("SS: ");
    uint32_to_hex(r.ss, buffer);
    kprint(buffer);
    kprint("\n");

    abort();
}

void enable_interrupts() {
    asm volatile("sti");
}

void irq_handler(registers_t r){
    //Send the EOI
    if (r.int_no >= 40) b_out(0xA0, 0x20); /* slave */
    b_out(0x20, 0x20); /* master */

    if (interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}

void register_handler(uint8_t index, isr_t handler) {
    interrupt_handlers[index] = handler;
}