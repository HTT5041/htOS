#include <abort.h>
#include <vga.h>

__attribute__((__noreturn__))
void abort() {

	kprint("\nabort(): Disabling interrupts");
	asm volatile("cli");
	kprint("\nabort(): Halting CPU");
	while(1){
		asm volatile("hlt");
	}
	__builtin_unreachable();
}