#ifndef LOGGING_H
#define LOGGING_H

#include <stdbool.h>

enum log_type {
    ERROR = 0,
    INFO = 1,
    SUCCESS = 2,
};

void log_message(enum log_type type, const char* message, bool nl);

void serial_message(const char* message);

#endif