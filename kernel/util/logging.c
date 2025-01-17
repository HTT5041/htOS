#include <logging.h>
#include <util.h>

#include <vga.h>
#include <ports.h>

void log_message(enum log_type type, const char* message, bool nl) {
    switch(type){
        case SUCCESS:
            terminal_set_colour(GREEN, BLACK);
            kprint("[SUCCESS] ");
            break;
        case ERROR:
            terminal_set_colour(RED, BLACK);
            kprint("[ERROR] ");
            break;
        case INFO:
            terminal_set_colour(LIGHT_BLUE, BLACK);
            kprint("[INFO] ");
            break;
    }
    terminal_set_colour(LIGHT_GREY, BLACK);
    kprint(message);
    if (nl) kprint("\n");
}

void serial_message(const char* message){
    int index = 0;

    while(message[index] != '\0'){
        b_out(0x3F8, message[index]);
        index++;
    }
}