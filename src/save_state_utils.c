//
// Created by Shadow-Link on 26/03/2023.
//

#include "debug.h"
#include "save_state_utils.h"

int raw_is_equal(char *save_data, int address, int requiredValue) {
    return save_data[address] == requiredValue;
}

int raw_is_greater_or_equal(char *save_data, int address, int requiredValue) {
    return save_data[address] >= requiredValue;
}

int raw_is_flag_set(char *save_data, int address, char flag) {
    return (save_data[address] & flag) == flag;
}
