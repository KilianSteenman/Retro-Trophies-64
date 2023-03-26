//
// Created by Kilian Steenman on 26/03/2023.
//

#ifndef N64_TROPHIES_SAVE_STATE_UTILS_H
#define N64_TROPHIES_SAVE_STATE_UTILS_H

#include <stdio.h>

int is_greater_or_equal(FILE *saveState, int address, int requiredValue);

int is_flag_set(FILE *saveState, int address, char flag);

#endif //N64_TROPHIES_SAVE_STATE_UTILS_H
