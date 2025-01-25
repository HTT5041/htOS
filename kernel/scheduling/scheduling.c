#include <scheduling.h>


task_t* current_task = NULL;
task_t* kernel_task = NULL;
int next_pid = 2;

int get_next_pid(){
	return next_pid++;
}

void init_task();

void init_kernel_task(void (*entry_point)()){
	kernel_task = create_task("kernel", 1, entry_point);
	current_task = kernel_task;

	kernel_task->next = NULL;
	kernel_task->prev = kernel_task;

	init_task();
}

void kclean_scheduler(){
	spinlock_acquire(&global_lock);
	// Remove any killed tasks and clean their memory
	task_t* temp = kernel_task;

	if(temp->killed){
		log_message(ERROR, "Kernel task killed aborting...", true);
		abort();
	}

	while(temp->next != NULL){
		temp = temp->next;
		if(temp->killed){
			// Unlink the task
			task_t *next_task = temp->next;
			if(next_task == NULL){
				temp->prev->next = NULL;
			} else {
				temp->prev->next = next_task;
				next_task->prev = temp->prev;
			}

			kfree((void*) temp->stack_base);
			kfree((void*) temp);
		}
	}
	spinlock_release(&global_lock);
}

void add_task(task_t* task){
	task_t* temp = current_task;
	while(temp->next != NULL){
		temp = temp->next;
	}
	temp->next = task;
	task->prev = temp;
}

void kill_task(){
	current_task->killed = true;
	yield(true);
}

void set_current_task_lock(bool lock){
    current_task->holds_lock = lock;
}

task_t* get_current_task(){
	return current_task;
}

void task_timer_callback(int pid){
	task_t* temp = kernel_task;

	while(temp != NULL) {
		if(temp->pid == pid){
			temp->sleeping = false;
			// Should wake the task by giving the next processor slice
			return;
		}
		temp = temp->next;
	}
}

void yield(bool yield_time){
    if(yield_time){
        current_task->time_remaining = 0;
    }
    asm volatile("int $0x81");
}

void init_task(){
    current_task->not_executed = false;
	asm volatile("mov %%eax, %%esp": :"a"(current_task->esp));
	asm volatile("pop %gs");
	asm volatile("pop %fs");
	asm volatile("pop %es");
	asm volatile("pop %ds");
	asm volatile("pop %ebp");
	asm volatile("pop %edi");
	asm volatile("pop %esi");
	asm volatile("pop %edx");
	asm volatile("pop %ecx");
	asm volatile("pop %ebx");
	asm volatile("pop %eax");
    asm volatile("sti");
	asm volatile("iret");
}

void scheduler(){

    if(current_task->time_remaining > 0){
        current_task->time_remaining--;
        return;
    }
    if(current_task->holds_lock) {
        return;
    }
    // Reset time slice
    current_task->time_remaining = DEFAULT_TIME_SLICE;

    asm volatile("push %eax");
	asm volatile("push %ebx");
	asm volatile("push %ecx");
	asm volatile("push %edx");
	asm volatile("push %esi");
	asm volatile("push %edi");
	asm volatile("push %ebp");
	asm volatile("push %ds");
	asm volatile("push %es");
	asm volatile("push %fs");
	asm volatile("push %gs");
	asm volatile("mov %%esp, %%eax":"=a"(current_task->esp));
    
	do {
		if(current_task->next != NULL){
			current_task = current_task->next;
		} else {
			current_task = kernel_task;
		}
	} while(current_task->killed || current_task->sleeping);

    if(current_task->not_executed){
		init_task();
		return;
	}

	asm volatile("mov %%eax, %%esp": :"a"(current_task->esp));
	asm volatile("pop %gs");
	asm volatile("pop %fs");
	asm volatile("pop %es");
	asm volatile("pop %ds");
	asm volatile("pop %ebp");
	asm volatile("pop %edi");
	asm volatile("pop %esi");
	asm volatile("pop %edx");
	asm volatile("pop %ecx");
	asm volatile("pop %ebx");
	asm volatile("pop %eax");
}