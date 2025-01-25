#ifndef VGA_H
#define VGA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

enum vga_colour {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	LIGHT_BROWN = 14,
	WHITE = 15,
};

void init_terminal();
void kprint(const char* str);
void kprint_char(const char c);
void terminal_set_colour(enum vga_colour foreground, enum vga_colour background);
void terminal_default_colour();
void terminal_clear();
void terminal_backspace(bool goto_eol);

#endif