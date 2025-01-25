#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stddef.h>
#include <ports.h>
#include <isr.h>

void init_timer(uint16_t freq);
void timer_irq_callback();
void sleep(int millis);

typedef void (*timer_callback_t)(int);

typedef struct {
    timer_callback_t callback;
    size_t interval;
    int id;
} timer_callback_entry_t;

size_t register_timer_callback(timer_callback_entry_t *cb);
void deregister_timer_callback(size_t index);

#endif