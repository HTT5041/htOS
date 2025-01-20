#include <util.h>

size_t strlen(const char* str){
    size_t size = 0;
    while (str[size]){
        size++;
    }
    return size;
}

void memcpy(void* src, void* dst, size_t size){
    uint32_t *d = (uint32_t*) dst;
    const uint32_t *s = (const uint32_t*) src;

    // Copy 4 bytes (1 word) at a time if possible
    while (size >= 4) {
        *d++ = *s++;
        size -= 4;
    }

    // Copy remaining bytes if n is not divisible by 4
    uint8_t *d_byte = (uint8_t*) d;
    const uint8_t *s_byte = (const uint8_t*) s;
    while (size--) {
        *d_byte++ = *s_byte++;
    }
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