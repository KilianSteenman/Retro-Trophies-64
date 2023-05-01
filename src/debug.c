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

void debug_print_number_and_stop(char *message, int value) {
#ifdef DEBUG
    char buffer[sizeof(message) + 11];
    sprintf(buffer, "%s%d", message, value);
    if (message != NULL) {
        printf(buffer);
    }

    console_render();
    while (1) {
        controller_scan();
        struct controller_data ckeys = get_keys_down();
        if (ckeys.c[0].A) {
            break;
        }
    }
#endif
}

void debug_print_and_pause(char *message) {
#ifdef DEBUG
    if (message != NULL) {
        printf(message);
    }

    console_render();
    while (1) {
        controller_scan();
        struct controller_data ckeys = get_keys_down();
        if (ckeys.c[0].A) {
            break;
        }
    }
#endif
}

void debug_print_and_stop(char *message) {
#ifdef DEBUG
    if (message != NULL) {
        printf(message);
    }

    console_render();
    while (1) {}
#endif
}
