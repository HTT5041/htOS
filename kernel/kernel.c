#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <gdt.h>
#include <isr.h>
#include <idt.h>

#include <vga.h>
#include <ports.h>
#include <util.h>
#include <keyboard.h>
#include <timer.h>

#include <string.h>
#include <abort.h>

#include <mem.h>

#include <logging.h>

#include <scheduling.h>
#include <task.h>

void kernel_process();

void kernel_main() {
    init_terminal();
    log_message(SUCCESS, "Terminal initialised", true);

    log_message(INFO, "Initialising GDT...", true);
    setup_gdt();
    log_message(SUCCESS, "GDT initialised", true);

    log_message(INFO, "Initialising ISR & IDT...", true);
    isr_install();
    log_message(SUCCESS, "ISR & IDT initialised", true);

    log_message(INFO, "Enabling interrupts...", true);
    enable_interrupts();
    log_message(SUCCESS, "interrupts enabled", true);

    log_message(INFO, "Initialising timer...", true);
    init_timer(1000);
    log_message(SUCCESS, "Timer initialised", true);

    log_message(INFO, "Initialising physical memory manager...", true);
    init_physical_memory();
    log_message(SUCCESS, "Physical memory initialised", true);

    register_handler(IRQ1, keyboard_handler);

    init_kernel_task(kernel_process);

    // Should never reach here
    abort();
}

void shell() {
    terminal_clear();

    terminal_set_colour(GREEN, BLACK);
    
    kprint("                     __          __       ______    ______  \n");
    kprint("                    /  |        /  |     /      \\  /      \\ \n");
    kprint("                    $$ |____   _$$ |_   /$$$$$$  |/$$$$$$  |\n");
    kprint("                    $$      \\ / $$   |  $$ |  $$ |$$ \\__$$/ \n");
    kprint("                    $$$$$$$  |$$$$$$/   $$ |  $$ |$$      \\ \n");
    kprint("                    $$ |  $$ |  $$ | __ $$ |  $$ | $$$$$$  |\n");
    kprint("                    $$ |  $$ |  $$ |/  |$$ \\__$$ |/  \\__$$ |\n");
    kprint("                    $$ |  $$ |  $$  $$/ $$    $$/ $$    $$/ \n");
    kprint("                    $$/   $$/    $$$$/   $$$$$$/   $$$$$$/  \n\n\n");
    

    terminal_default_colour();

    kprint("CPU Model: ");
    kprint(get_cpu_name());

    kprint("\n\n\n> ");
    while(1) {
        kprint("hi");
        sleep(1000);
    }
}

void kernel_process(){
    // Create shell process
    task_t* shell_task = create_task("shell", get_next_pid(), shell);
    add_task(shell_task);


    while(1) {
        // Kernel loop 
        kclean_scheduler();
    }
}