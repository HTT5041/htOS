#include <scheduling.h>

typedef struct {
    registers_t cpu_state;
    struct task_t* next;
} task_t;

task_t* first_task;
task_t* current_task;

void scheduler(registers_t regs) {
    if (current_task) {
        current_task->cpu_state = regs;
        current_task = current_task->next;
    } else {
        current_task = first_task;
    }

    //regs = current_task->cpu_state;
}