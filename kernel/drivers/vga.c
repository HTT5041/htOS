#include <vga.h>
#include <ports.h>

#include <util.h>

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

size_t VGA_MAX_WIDTH = 80;
size_t VGA_MAX_HEIGHT = 25;

size_t current_x;
size_t current_y;

uint16_t* VGA_MEMORY = (uint16_t*) 0xB8000;
uint8_t terminal_colour;

int get_xy_index(int x, int y) {
    return y * VGA_MAX_WIDTH + x;
}

void set_cursor(int x, int y){
    int loc = get_xy_index(x, y) + 1;

    b_out(REG_SCREEN_CTRL, 14);
    b_out(REG_SCREEN_DATA, (unsigned char)(loc >> 8));
    b_out(REG_SCREEN_CTRL, 15);
    b_out(REG_SCREEN_DATA, (unsigned char)(loc & 0xff));
}

uint8_t vga_colour(enum vga_colour foreground, enum vga_colour background) {
    return foreground | background << 4;
}

void terminal_put_char_at(int x, int y, char c) {
    VGA_MEMORY[get_xy_index(x, y)] = (uint16_t) c | (uint16_t) terminal_colour << 8;
    set_cursor(x, y);
}

char get_char_at(int x, int y){
    return (VGA_MEMORY[get_xy_index(x, y)] & 0x00FF);
}

void do_scroll(){
    // First, shift the lines up by one
    memcpy((uint8_t*) VGA_MEMORY + (VGA_MAX_WIDTH * 2),              // SRC (Beginning of the 2nd row)
           (uint8_t*) VGA_MEMORY,                                    // DST (Beginning of the 1st row)                 
           (VGA_MAX_HEIGHT * VGA_MAX_WIDTH - VGA_MAX_WIDTH) * 2); // Size 

    // Now delete the last line
    for(size_t i = 0; i < VGA_MAX_WIDTH; i++) {
        VGA_MEMORY[(VGA_MAX_HEIGHT * VGA_MAX_WIDTH - VGA_MAX_WIDTH) + i] = ' ' | terminal_colour << 8;
    }
    current_y = VGA_MAX_HEIGHT - 1;
}

void terminal_write_char(char c){
    if(c == '\n'){
        current_x = 0;

        if (++current_y == VGA_MAX_HEIGHT) {
            do_scroll();
        }

        set_cursor(current_x-1, current_y);
        return;
    }

    terminal_put_char_at(current_x, current_y, c);

    // Update cursor pos
    if (++current_x == VGA_MAX_WIDTH) {
        current_x = 0;
        if (++current_y == VGA_MAX_HEIGHT) {
            do_scroll();
        }
    }
}

void terminal_write(const char* str, size_t size) {
    for (size_t i = 0; i < size; i++){
        terminal_write_char(str[i]);
    }
}

void terminal_write_str(const char* str) {
    terminal_write(str, strlen(str));
}

// Public functions --------------------------------------------------------------------------------------------------------------------

void init_terminal() {
    current_x = 0;
    current_y = 0;
    terminal_colour = vga_colour(LIGHT_GREY, BLACK);
    for (size_t y = 0; y < VGA_MAX_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_MAX_WIDTH; x++) {
            terminal_put_char_at(x, y, ' '); //Clear the screen
        }
    }
}

void kprint(const char* str){
    terminal_write_str(str);
}

void terminal_set_colour(enum vga_colour foreground, enum vga_colour background){
    terminal_colour = vga_colour(foreground, background);
}

void terminal_default_colour(){
    terminal_set_colour(LIGHT_GREY, BLACK);
}

void terminal_clear() {
    for (size_t y = 0; y < VGA_MAX_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_MAX_WIDTH; x++) {
            terminal_put_char_at(x, y, ' ');
        }
    }
    current_x = 0;
    current_y = 0;
    set_cursor(-1, 0);
}

void terminal_backspace(bool goto_eol){

    if (current_x == 0 && current_y == 0){
        // Nothing to backspace
        return;
    } else if (current_x == 0){
        current_y -= 1;
        current_x = VGA_MAX_WIDTH;
        if (goto_eol){
            // Back track until a non ' ' character is found
            while(current_x > 0){
                char char_at = get_char_at(current_x, current_y);
                if (char_at != ' '){
                    current_x++;
                    break;
                }
                current_x--;
            }
            set_cursor(current_x-1, current_y);
            return;
        }
    }
    current_x -= 1;
    terminal_put_char_at(current_x, current_y, ' ');
    set_cursor(current_x-1, current_y);
}

void kprint_char(const char c){
    terminal_write_char(c);
}