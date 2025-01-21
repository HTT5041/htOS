#include <timer.h>

#include <vga.h>
#include <util.h>

uint64_t count_down = 0;

#define MAX_TIMER_CALLBACKS 128
timer_callback_entry* callbacks[MAX_TIMER_CALLBACKS];
size_t max_interval = 0;
size_t current_tick = 0;

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

void timer_irq_callback(registers_t regs){
    current_tick++;

    if(current_tick > max_interval){
        current_tick = 0;
    }

    // Iterate over all callbacks : This is quite inefficient, maybe we could use a bit map 
    if (current_tick != 0){
        for (size_t i = 0; i < MAX_TIMER_CALLBACKS; i++){
            if(callbacks[i]){
                size_t interval = callbacks[i]->interval;
                if(current_tick % interval == 0){
                    timer_callback callback = callbacks[i]->callback;
                    callback();
                }
            }
        }
    }

    if(count_down == 0) return;
    count_down--;
}

size_t register_timer_callback(timer_callback_entry *cb){
    if (cb->interval > max_interval){
        max_interval = cb->interval;
    }

    for (size_t i = 0; i < MAX_TIMER_CALLBACKS; i++) {
        if(!(callbacks[i])){
            callbacks[i] = cb;
            return i;
        }
    }
    return -1;
}

void deregister_timer_callback(size_t index){
    if(index > MAX_TIMER_CALLBACKS) return;

    callbacks[index] = 0;
}

void sleep(int millis){
    count_down = millis;
    while (count_down != 0){
        asm volatile("hlt");
    }
}