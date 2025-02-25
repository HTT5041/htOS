#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <mem.h>

void uint64_to_hex(uint64_t num, char buffer[17]);
void uint32_to_hex(uint32_t num, char buffer[9]);
void char_to_hex(char c, char buffer[3]);

char* get_cpu_name();

#endif