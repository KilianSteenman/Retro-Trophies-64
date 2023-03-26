//
// Created by Kilian Steenman on 26/03/2023.
//

#include <libdragon.h>

#include "graphics_ext.h"

void graphics_draw_number(display_context_t disp, int x, int y, int number) {
    char buffer[11];
    sprintf(buffer, "%d", number);
    graphics_draw_text(disp, x, y, buffer);
}

void graphics_draw_progressbar(display_context_t disp, int x, int y, int width, int height, uint32_t bgColor,
                               uint32_t fillColor, int percentageCompleted) {
    graphics_draw_box(disp, x, y, width, height, bgColor);
    graphics_draw_box(disp, x, y, width * ((float) percentageCompleted / 100.0f), height, fillColor);
}