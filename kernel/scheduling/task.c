#include <task.h>

#define STACK_SIZE 0x1000

task_t* create_task(char* name, int pid, void (*entry_point)()){
    task_t* task = (task_t*) kalloc();

    task->name = name;
    task->pid = pid;
    task->stack_base = (uint32_t)kalloc();

    uint32_t *stack = (uint32_t *)(task->stack_base + STACK_SIZE);
    *(--stack) = (uint32_t)0x202; // EFLAGS
    *(--stack) = (uint32_t)0x08; // CS
    *(--stack) = (uint32_t)entry_point; // EIP
    *(--stack) = 0; // eax
	*(--stack) = 0; // ebx
	*(--stack) = 0; // ecx
	*(--stack) = 0; // edx
	*(--stack) = 0; // esi
	*(--stack) = 0; // edi
	*(--stack) = task->stack_base + STACK_SIZE; //ebp
	*(--stack) = 0x10; // ds
	*(--stack) = 0x10; // fs
	*(--stack) = 0x10; // es
	*(--stack) = 0x10; // gs

    task->esp = (uint32_t)stack;
    task->not_executed = true;
    task->holds_lock = false;
    task->time_remaining = DEFAULT_TIME_SLICE;

    task->next = NULL;
    task->prev = NULL;

    return task;
}