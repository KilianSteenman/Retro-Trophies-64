//
// Created by Shadow-Link on 25/03/2023.
//

#include <libdragon.h>
#include <stdarg.h>
#include <stdio.h>

void debug_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

#ifdef DEBUG
    vprintf(format, args);
#endif

    va_end(args);
}

void debug_printf_and_stop(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    while (1) {}
}

void debug_printf_and_pause(const char *format, ...) {
#ifdef DEBUG
    va_list args;
    va_start(args, format);
    debug_printf(format, args);
    va_end(args);

    while (1) {
        controller_scan();
        struct controller_data ckeys = get_keys_down();
        if (ckeys.c[0].A) {
            break;
        }
    }
#endif
}
