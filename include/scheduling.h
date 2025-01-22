#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <stdint.h>
#include <isr.h>

typedef void (*task_entry_t)(void);

void init_scheduler();

void add_task(task_entry_t task);

#endif