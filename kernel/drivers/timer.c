#include <timer.h>

#include <vga.h>
#include <util.h>

uint64_t count_down = 0;

void init_timer(uint16_t freq){
    asm volatile("cli");

    uint16_t divisor = 1193180 / freq;

    uint8_t high = (divisor >> 8) & 0xFF;
    uint8_t low = divisor & 0xFF;

    b_out(0x43, 0x36);
    b_out(0x40, low);
    b_out(0x40, high);

    asm volatile("sti");
}

void timer_callback(registers_t regs){
    if(count_down == 0) return;
    count_down--;
}

void sleep(int millis){
    count_down = millis;
    while (count_down != 0){
        asm volatile("hlt");
    }
}