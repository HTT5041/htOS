#include <util.h>

void cpuid(uint32_t code, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
    asm volatile("cpuid"
                 : "=a" (*a), "=b" (*b), "=c" (*c), "=d" (*d)
                 : "a" (code));
}

char model_name[49] = {0};

char* get_cpu_name() {
    uint32_t regs[4];

    // Get the CPU model name using 3 CPUID calls
    cpuid(0x80000002, &regs[0], &regs[1], &regs[2], &regs[3]);
    memcpy(regs, model_name, sizeof(regs));
    cpuid(0x80000003, &regs[0], &regs[1], &regs[2], &regs[3]);
    memcpy(regs, model_name + 16,  sizeof(regs));
    cpuid(0x80000004, &regs[0], &regs[1], &regs[2], &regs[3]);
    memcpy(regs, model_name + 32, sizeof(regs));

    return model_name;
}

void uint64_to_hex(uint64_t num, char buffer[17]) {
    const char hex_digits[] = "0123456789abcdef";

    // Start from the end of the buffer and work backwards
    for (int i = 15; i >= 0; --i) {
        buffer[i] = hex_digits[num & 0xF]; // Extract the last 4 bits and find the corresponding hex digit
        num >>= 4; // Shift right by 4 bits
    }

    buffer[16] = '\0'; // Null-terminate the string
}

void uint32_to_hex(uint32_t num, char buffer[9]) {
    const char hex_digits[] = "0123456789abcdef";

    // Start from the end of the buffer and work backwards
    for (int i = 7; i >= 0; --i) {
        buffer[i] = hex_digits[num & 0xF]; // Extract the last 4 bits and find the corresponding hex digit
        num >>= 4; // Shift right by 4 bits
    }

    buffer[8] = '\0'; // Null-terminate the string
}

void char_to_hex(char c, char buffer[3]) {
    const char hex_digits[] = "0123456789abcdef";

    char lower_nibble = hex_digits[c & 0xF];
    c >>= 4;
    char upper_nibble = hex_digits[c & 0xF];
    
    buffer[0] = upper_nibble;
    buffer[1] = lower_nibble;
    buffer[2] = '\0';
}