#include <stdint.h>

#include <gdt.h>
#include <logging.h>

struct GDTEntry {
    uint16_t limit_low;  // Lower 16 bits of the segment limit
    uint16_t base_low;   // Lower 16 bits of the base address
    uint8_t base_middle; // Next 8 bits of the base address
    uint8_t access;      // Access flags
    uint8_t flags; // Limit and granularity flags
    uint8_t base_high;   // Last 8 bits of the base address
} __attribute__((packed)); // Ensure no padding

struct GDTPointer {
    uint16_t limit;  // Size of the GDT - 1
    uint32_t base;   // Base address of the GDT
} __attribute__((packed));

#define GDT_SIZE 3
struct GDTEntry gdt[GDT_SIZE];
struct GDTPointer gdtp;

void set_gdt_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    gdt[index].base_low = base & 0xFFFF;
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;

    gdt[index].limit_low = limit & 0xFFFF;
    gdt[index].flags = (limit >> 16) & 0x0F;
    gdt[index].flags |= (flags & 0xF0);
    gdt[index].access = access;
}

void load_gdt() {
    __asm__ volatile ("lgdt (%0)" : : "r" (&gdtp));
}

void setup_gdt() {
    // Set up the GDT pointer
    gdtp.limit = (sizeof(struct GDTEntry) * GDT_SIZE) - 1;
    gdtp.base = (uint32_t)&gdt;

    // Null descriptor (required)
    log_message(INFO, "Setting NULL GDT entry", true);
    set_gdt_entry(0, 0, 0, 0, 0);

    // Code segment descriptor
    log_message(INFO, "Setting code GDT entry", true);
    set_gdt_entry(1, 0, 0xFFFFF, 0x9F, 0xCF); // Base 0, Limit 4 GB, Code segment
    // 0x9A:
    // 1 00 1 1 0 1 0 (Executable, Readable)

    // Data segment descriptor
    log_message(INFO, "Setting data GDT entry", true);
    set_gdt_entry(2, 0, 0xFFFFF, 0x92, 0xCF); // Base 0, Limit 4 GB, Data segment
    // 0x92
    // 1 00 1 0 0 1 0 (Writable, Non-exec.)

    // 0xCF flag = 32bit protected, 4kb page

    // See https://wiki.osdev.org/GDT_Tutorial

    // Load the GDT
    log_message(INFO, "Loading the GDT", true);
    load_gdt();
}