#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <stdint.h>
#include <stdbool.h>
#include <isr.h>
#include <mem.h>
#include <vga.h>
#include <util.h>
#include <timer.h>
#include <logging.h>
#include <spinlock.h>
#include <task.h>
#include <abort.h>

void kclean_scheduler();

int get_next_pid();

void scheduler();
void set_current_task_lock(bool lock);
void yield(bool yield_time);

void init_kernel_task(void (*entry_point)());

void add_task(task_t* task);
void kill_task();

task_t* get_current_task();
void task_timer_callback(int pid);

#endif