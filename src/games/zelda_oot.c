//
// Created by Shadow-Link on 16/04/2023.
//

#include <stdio.h>

#include "zelda_oot.h"

#include "../debug.h"
#include "../trophy.h"
#include "../game.h"
#include "../save_state_utils.h"

int get_skulltula_count(char *save_data) {
    // TODO: Actually verify skulltula flags instead of the counter to prevent the duplicate skulltula glitch
    return read_short(save_data, 0xF0);
}

void get_trophy_data_zelda_oot(Game *game, char *save_data) {
    add_bool_trophy(game, "A wimp is still a wimp", "Get access to the Great Deku Tree", BRONZE,
                    raw_is_flag_set(save_data, 0xEF5, 0b00010000));
    add_bool_trophy(game, "Good...bye...", "Obtain the Kokiri Emerald", SILVER,
                    raw_is_flag_set(save_data, 0x0C5, 0b00000100));
    add_bool_trophy(game, "Sworn Brothers", "Obtain the Goron's Ruby", SILVER,
                    raw_is_flag_set(save_data, 0x0C5, 0b00001000));
    add_bool_trophy(game, "Don't tell my father...", "Obtain Zora's Sapphire", SILVER,
                    raw_is_flag_set(save_data, 0x0C5, 0b00010000));
    add_bool_trophy(game, "Creepy little buggers", "Collect your first Gold Skulltula", BRONZE,
                    raw_is_flag_set(save_data, 0x0C5, 0b10000000));
    add_counter_trophy(game, "At least it's not 900", "Collect all Gold Skulltulas", SILVER,
                       100, get_skulltula_count(save_data));
}
