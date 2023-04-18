//
// Created by Shadow-Link on 26/03/2023.
//

#ifndef N64_TROPHIES_SAVE_STATE_UTILS_H
#define N64_TROPHIES_SAVE_STATE_UTILS_H

#include <stdio.h>

int raw_is_equal(char *save_data, int address, int requiredValue);

int raw_is_greater_or_equal(char *save_data, int address, int requiredValue);

int raw_is_flag_set(char *save_data, int address, char flag);

#endif //N64_TROPHIES_SAVE_STATE_UTILS_H
