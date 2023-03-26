//
// Created by Shadow-Link on 25/03/2023.
//

#include <libdragon.h>
#include <stdio.h>

void debug_print_number_and_stop(char *message, int value) {
    char buffer[sizeof(message) + 11];
    sprintf(buffer, "%s%d", message, value);
    if (message != NULL) {
        printf(buffer);
    }

    console_render();
    while (1) {}
}

void debug_print_and_stop(char *message) {
    if (message != NULL) {
        printf(message);
    }

    console_render();
    while (1) {}
}
