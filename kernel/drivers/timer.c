#include <timer.h>

#include <vga.h>
#include <util.h>
#include <scheduling.h>

uint64_t count_down = 0;

#define MAX_TIMER_CALLBACKS 128
timer_callback_entry_t* callbacks[MAX_TIMER_CALLBACKS];
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

void timer_irq_callback(){
    b_out(0x20, 0x20);
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
                    timer_callback_t callback = callbacks[i]->callback;
                    callback(callbacks[i]->id);
                    deregister_timer_callback(i);
                }
            }
        }
    }
}

size_t register_timer_callback(timer_callback_entry_t *cb){
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
    spinlock_acquire(&global_lock);
    task_t* ct = get_current_task();
    ct->sleeping = true;

    timer_callback_entry_t *tcb = kalloc();
    tcb->callback = task_timer_callback;
    tcb->interval = millis;
    tcb->id = ct->pid;

    register_timer_callback(tcb);
    spinlock_release(&global_lock);
    yield(true);
}