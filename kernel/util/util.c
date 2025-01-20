#include <util.h>

size_t strlen(const char* str){
    size_t size = 0;
    while (str[size]){
        size++;
    }
    return size;
}

void memcpy(char* src, char* dst, size_t size){
    for(size_t i = 0; i < size; i++){
        dst[i] = src[i];
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