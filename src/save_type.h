//
// Created by Shadow-Link on 01/05/2023.
//

#ifndef N64_TROPHIES_SAVE_TYPE_H
#define N64_TROPHIES_SAVE_TYPE_H

typedef enum {
    RAM,
    EEP
} SaveType;

extern const int EEP_SIZE;
extern const int RAM_SIZE;

extern const int LARGEST_SAVE_DATA_SIZE;

char *get_extension_for_save_type(SaveType saveType);

int get_size_of_save_data(SaveType saveType);

#endif //N64_TROPHIES_SAVE_TYPE_H
