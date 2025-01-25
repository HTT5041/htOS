#ifndef TASK_H
#define TASK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <mem.h>

#define DEFAULT_TIME_SLICE 10

typedef struct task_t {
    char* name;
    int pid;
    uint32_t esp;
    uint32_t stack_base;
    bool not_executed;
    bool holds_lock;
    int time_remaining;
    bool killed;
    bool sleeping;
    struct task_t *next, *prev;
} task_t;

task_t* create_task(char* name, int pid, void (*entry_point)());

#endif