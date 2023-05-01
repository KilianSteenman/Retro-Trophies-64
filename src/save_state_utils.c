//
// Created by Shadow-Link on 26/03/2023.
//

#include "debug.h"
#include "save_state_utils.h"

int raw_is_equal(char *save_data, int address, int requiredValue) {
    return save_data[address] == requiredValue;
}

int raw_is_greater_or_equal(char *save_data, int address, int requiredValue) {
    int value = save_data[address];
    debug_printf("Compare %d >= %d\n", value, requiredValue);
    return value >= requiredValue;
}

int raw_is_flag_set(char *save_data, int address, char flag) {
    debug_printf("Flag %d = %d\n", save_data[address], flag);
    return (save_data[address] & flag) == flag;
}

int read_short(char *save_data, int address) {
    return (((short) save_data[address + 1]) << 8) | save_data[address];
}
