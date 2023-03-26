//
// Created by Kilian Steenman on 26/03/2023.
//

#include "save_state_utils.h"

int is_greater_or_equal(FILE *saveState, int address, int requiredValue) {
    unsigned short value = 0;
    fseek(saveState, address, SEEK_SET);
    fread(&value, 1, sizeof(value), saveState);
    return value >= requiredValue;
}

int is_flag_set(FILE *saveState, int address, char flag) {
    unsigned char value = 0;
    fseek(saveState, address, SEEK_SET);
    fread(&value, 1, sizeof(value), saveState);
    return (value & flag) == flag;
}
