#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str);

void memcpy(char* src, char* dst, size_t size);

void uint64_to_hex(uint64_t num, char buffer[17]);
void uint32_to_hex(uint32_t num, char buffer[9]);

#endif