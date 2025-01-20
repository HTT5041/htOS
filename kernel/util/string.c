#include <string.h>
#include <stdbool.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

bool streq(const char* a, const char* b){
    if (a == NULL || b == NULL) {
        return false;
    }

    // Compare character by character
    while (*a != '\0' && *b != '\0') {
        if (*a != *b) {
            return false; // Mismatch found
        }
        a++;
        b++;
    }

    // Ensure both strings reached the null terminator
    return *a == '\0' && *b == '\0';
}