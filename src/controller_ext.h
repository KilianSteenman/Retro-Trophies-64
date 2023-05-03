//
// Created by Shadow-Link on 03/05/2023.
//

#ifndef N64_TROPHIES_CONTROLLER_EXT_H
#define N64_TROPHIES_CONTROLLER_EXT_H

#include <libdragon.h>
#include <stdbool.h>

bool is_down_pressed(struct controller_data down);
bool is_up_pressed(struct controller_data down);

#endif //N64_TROPHIES_CONTROLLER_EXT_H
