#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <stdint.h>
#include <stdbool.h>
#include <isr.h>
#include <mem.h>
#include <vga.h>
#include <util.h>
#include <timer.h>

void yield();

void init_scheduler();

#endif