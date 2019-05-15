#include "defs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void cse320_print(char* message) {
    printf("%s", message);
}

void cse320_printf(const char *fmt, ...) {
    int size = 0;
    char *p = NULL;
    va_list ap;

    va_start(ap, fmt);
    size = vsnprintf(p, size, fmt, ap);
    va_end(ap);

    if (size < 0)
        return;

    size++;
    p = malloc(size * sizeof(*p));
    if (!p)
        return;

    va_start(ap, fmt);
    size = vsnprintf(p, size, fmt, ap);
    va_end(ap);

    if (size < 0) {
        free(p);
        return;
    }

    cse320_print(p);
    free(p);
}
