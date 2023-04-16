//
// Created by Shadow-Link on 16/04/2023.
//

#include <stdio.h>

#include "zelda_oot.h"

#include "../trophy.h"
#include "../game.h"
#include "../save_state_utils.h"

void get_trophy_data_zelda_oot(Game *game, FILE *saveState) {
    char save_data[32768];
    if (fread(&save_data, sizeof(save_data), 1, saveState)) {
        printf("Read Zelda OOT save game into memory\n");
    }

    add_bool_trophy(game, "It's a me", "Unlock Luigi", SILVER,
                    raw_is_flag_set(save_data, 0x457, 0b1));
}
