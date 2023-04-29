//
// Created by Shadow-Link on 26/03/2023.
//

#ifndef N64_TROPHIES_GRAPHICS_EXT_H
#define N64_TROPHIES_GRAPHICS_EXT_H

#include <stdio.h>

void graphics_draw_number(display_context_t disp, int x, int y, int number);

void graphics_draw_progressbar(display_context_t disp, int x, int y, int width, int height, uint32_t bgColor,
                               uint32_t fillColor, int percentageCompleted);

void graphics_draw_bordered_box(display_context_t disp, int x, int y, int width, int height, int backgroundColor,
                                int borderColor, int borderThickness);

#endif //N64_TROPHIES_GRAPHICS_EXT_H