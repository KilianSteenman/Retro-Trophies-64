//
// Created by Shadow-Link on 01/05/2023.
//

#include "save_type.h"

const int EEP_SIZE = 512;
const int RAM_SIZE = 32768;

const int LARGEST_SAVE_DATA_SIZE = 32768;

char *get_extension_for_save_type(SaveType saveType) {
    switch (saveType) {
        case RAM:
            return "srm";
        case EEP:
            return "eep";
        default:
            return "ram";
    }
}

int get_size_of_save_data(SaveType saveType) {
    switch (saveType) {
        case RAM:
            return RAM_SIZE;
        case EEP:
            return EEP_SIZE;
        default:
            return RAM_SIZE;
    }
}
