//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdio.h>
#include <string.h>

#include "../trophy.h"
#include "../game.h"
#include "../save_state_utils.h"

void get_game_data_super_smash_bros(Game *game, FILE *saveState) {
    strcpy(game->title, "Super Smash Bros");
    add_bool_trophy(game, "It's a me", "Unlock Luigi", SILVER,
                    is_flag_set(saveState, 0x457, 0b1));
    add_bool_trophy(game, "I'll take on anybody, anytime", "Unlock Captain Falcon", SILVER,
                    is_flag_set(saveState, 0x457, 0b100));
    add_bool_trophy(game, "Fuzzy pickles", "Unlock Ness", SILVER,
                    is_flag_set(saveState, 0x457, 0b10));
    add_bool_trophy(game, "Sweet dreams", "Unlock Jigglypuff", SILVER,
                    is_flag_set(saveState, 0x457, 0b1000));
}
