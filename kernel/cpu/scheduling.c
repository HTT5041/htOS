#include <scheduling.h>

#define MAX_TASKS 32
#define STACK_SIZE 0x1000

typedef struct {
    uint8_t *stack;
    uint8_t *stack_pointer;
    void (*entry)(void);
    int active;
} task_t;

task_t tasks[MAX_TASKS];
int current_task = -1;
int task_count = 0;

void create_task(void (*entry)(void)) {
    if (task_count >= MAX_TASKS) {
        return;
    }

    task_t* task = &tasks[task_count++];
    task->stack = kalloc();
    task->stack_pointer = task->stack + STACK_SIZE;

    *(uint32_t *)(task->stack_pointer) = (uint32_t)entry;  // Fake return address
    task->stack_pointer -= sizeof(uint32_t);
    task->active = 1;
}

void yield() {
    static uint32_t saved_sp;

    asm volatile (
        "mov %%esp, %0\n\t"  // Save current stack pointer
        "mov %1, %%esp\n\t"  // Load the next task's stack pointer
        : "=r" (saved_sp)
        : "r" (tasks[current_task].stack_pointer)
    );

    // Save current task's stack pointer
    tasks[current_task].stack_pointer = (uint8_t *)saved_sp;

    // Find the next active task
    do {
        current_task = (current_task + 1) % task_count;
    } while (!tasks[current_task].active);

    // Load the next task's stack pointer
    asm volatile (
        "mov %0, %%esp\n\t"  // Load the next task's stack pointer
        :
        : "r" (tasks[current_task].stack_pointer)
    );
}

void task_exit() {
    tasks[current_task].active = 0;

    kfree(tasks[current_task].stack);

    yield();
}

void task1() {
    while(1){
        kprint("1");
        yield();
    }
    task_exit();
}

void task2() {
    while(1){
        kprint("2");
        yield();
    }
    task_exit();
}

void init_scheduler() {
    create_task(task1);
    create_task(task2);


    yield();
}