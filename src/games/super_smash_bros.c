//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdio.h>
#include <string.h>

#include "../game.h"
#include "../debug.h"
#include "../test.h"

int is_flag_set(FILE *saveState, int address, char flag) {
    unsigned char value = 0;
    fseek(saveState, address, SEEK_SET);
    fread(&value, 1, sizeof(value), saveState);
    return (value && flag) == flag;
}

void get_game_data_super_smash_bros(Game *game, FILE *saveState) {
    strcpy(game->title, "Super Smash Bros");
    add_trophy(game, "It's a me", "Unlock Luigi", SILVER, BOOL,
               is_flag_set(saveState, 0x457, 0b1));
    add_trophy(game, "I'll take on anybody, anytime", "Unlock Captain Falcon", SILVER, BOOL,
               is_flag_set(saveState, 0x457, 0b100));
    add_trophy(game, "Fuzzy pickles", "Unlock Ness", SILVER, BOOL,
               is_flag_set(saveState, 0x457, 0b10));
    add_trophy(game, "Sweet dreams", "Unlock Jigglypuff", SILVER, BOOL,
               is_flag_set(saveState, 0x457, 0b1000));
}