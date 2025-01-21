#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stddef.h>
#include <ports.h>
#include <isr.h>

void init_timer(uint16_t freq);
void timer_irq_callback(registers_t regs);
void sleep(int millis);

typedef void (*timer_callback)(void);

typedef struct {
    timer_callback callback;
    size_t interval;
} timer_callback_entry;

size_t register_timer_callback(timer_callback_entry *cb);
void deregister_timer_callback(size_t index);

#endif