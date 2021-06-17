#ifndef chess_exception_h
#define chess_exception_h

#include "stdio.h"
#include "stdlib.h"

void exception (const char*, const char*, int);

void exception (const char* message, const char* file, int line) {
    fprintf(stderr, "Exception raised!\n"
                    "   File: %s\n"
                    "   Line: %d\n"
                    "Message: %s\n",
                    file, line, message);
    exit(-1);
}

#endif
