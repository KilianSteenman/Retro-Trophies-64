//
// Created by Shadow-Link on 30/04/2023.
//

#ifndef N64_TROPHIES_UI_H
#define N64_TROPHIES_UI_H

uint32_t SCREEN_BACKGROUND_COLOR;
uint32_t SCREEN_TITLE_COLOR;
uint32_t TOTALS_COLOR;

uint32_t TILE_DEFAULT_BACKGROUND_COLOR;
uint32_t TILE_SELECTED_BACKGROUND_COLOR;
uint32_t TILE_DEFAULT_BORDER_COLOR;
uint32_t TILE_SELECTED_BORDER_COLOR;
uint32_t TILE_DEFAULT_TEXT_COLOR;
uint32_t TILE_SELECTED_TEXT_COLOR;

uint32_t PROGRESSBAR_FOREGROUND_COLOR;
uint32_t PROGRESSBAR_BACKGROUND_COLOR;

void init_colors() {
    SCREEN_BACKGROUND_COLOR = graphics_make_color(197, 194, 196, 255);
    SCREEN_TITLE_COLOR = graphics_make_color(101, 101, 101, 255);
    TOTALS_COLOR = graphics_make_color(255, 0, 0, 255);

    TILE_DEFAULT_BACKGROUND_COLOR = graphics_make_color(197, 194, 196, 255);
    TILE_SELECTED_BACKGROUND_COLOR = graphics_make_color(14, 0, 17, 255);
    TILE_DEFAULT_BORDER_COLOR = graphics_make_color(101, 101, 101, 255);
    TILE_SELECTED_BORDER_COLOR = graphics_make_color(255, 255, 255, 255);
    TILE_DEFAULT_TEXT_COLOR = graphics_make_color(0, 0, 255, 255);
    TILE_SELECTED_TEXT_COLOR = graphics_make_color(255, 255, 255, 255);

    PROGRESSBAR_FOREGROUND_COLOR = graphics_make_color(255, 0, 0, 255);
    PROGRESSBAR_BACKGROUND_COLOR = graphics_make_color(255, 255, 255, 255);
}

#endif //N64_TROPHIES_UI_H