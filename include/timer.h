#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <ports.h>
#include <isr.h>

void init_timer(uint16_t freq);
void timer_callback(registers_t regs);
void sleep(int millis);

#endif