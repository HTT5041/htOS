
.PHONY: clean

ASM = /home/harry/opt/cross/bin/i686-elf-as
GCC = /home/harry/opt/cross/bin/i686-elf-gcc

C_SOURCES = $(wildcard kernel/*.c kernel/drivers/*.c kernel/util/*.c kernel/cpu/*.c)
ASM_SOURCES = $(wildcard boot/*.s kernel/cpu/*.s)
C_OBJS = ${C_SOURCES:.c=.o}
ASM_OBJS = ${ASM_SOURCES:.s=.o}

ASM_FLAGS = --gstabs+
GCC_FLAGS = -Iinclude -Werror -std=gnu99 -ffreestanding -Wall -Wextra -g -Wno-unused-parameter
LINKER_FLAGS = -T linker.ld -ffreestanding -nostdlib -lgcc -g

BIN = htos.bin

all: $(BIN)

debug: $(BIN)
	qemu-system-i386 -serial stdio -s -S -kernel $(BIN)

run: $(BIN)
	qemu-system-i386 -serial stdio -kernel $(BIN)

clean:
	@find . -name '*.o' -type f -delete
	@rm -f *.bin

$(BIN): ${C_OBJS} ${ASM_OBJS}
	$(GCC) $(LINKER_FLAGS) -o $@ $^


%.o: %.c
	${GCC} ${GCC_FLAGS} -c $< -o $@

%.o: %.s
	$(ASM) $(ASM_FLAGS) -o $@ $<

%.o: %.asm
	nasm -f elf -o $@ $<