#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


void memcpy(void* src, void* dst, size_t size);

void* kalloc();
void kfree(void* frame);
void init_physical_memory();

#endif