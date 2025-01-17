#include <ports.h>

void b_out(unsigned short port, unsigned char data){
    __asm__ volatile ("out %%al, %%dx" : : "a" (data), "d" (port));
}


unsigned char b_in(unsigned short port){
    unsigned char result;
    __asm__ volatile ("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}