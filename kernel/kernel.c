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

#include <logging.h>

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

    terminal_set_colour(RED, BLACK);
    
    
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

    register_handler(IRQ1, keyboard_handler);

    while (1){
        asm volatile("hlt");
    }
}