//
// Created by Shadow-Link on 22/06/2023.
//

#include <stdio.h>

#include "../debug.h"
#include "../save_state_utils.h"
#include "../game.h"

int get_total_balloon_count(char *save_data) {
    // Total balloon count for save slot 1
    return (read_byte(save_data, 0xE) & 0b11111110) >> 1;
}

int get_key_count(char *save_data) {
    return raw_is_flag_set(save_data, 0x48, 0b00000010) +
            raw_is_flag_set(save_data, 0x48, 0b00000100) +
            raw_is_flag_set(save_data, 0x48, 0b00001000) +
            raw_is_flag_set(save_data, 0x48, 0b00010000);
}

bool has_first_balloon(char *save_data) {
    return get_total_balloon_count(save_data) >= 1;
}

void get_game_data_diddy_kong_racing(Game *game, char *save_data) {
    add_bool_trophy(game, "First balloon", "Collect a golden balloon", BRONZE, has_first_balloon(save_data));
    add_counter_trophy(game, "All balloons", "Collect all golden balloons", BRONZE, 47, get_total_balloon_count(save_data));
    add_counter_trophy(game, "Beat all levels", "Beat all levels", BRONZE, 50, 0);
    add_counter_trophy(game, "Beat all silver coin", "Beat all silver coin", BRONZE, 50, 0);
    add_counter_trophy(game, "Got all golden trophies", "Got all golden trophies", BRONZE, 5, 0);
    add_counter_trophy(game, "Found all hidden keys", "Found all hidden keys", SILVER, 4, get_key_count(save_data));
    add_bool_trophy(game, "Beat dino domain", "First balloon", BRONZE, true);
    add_bool_trophy(game, "Beat sherbet island", "First balloon", BRONZE, true);
    add_bool_trophy(game, "Beat snowflake mountain", "First balloon", BRONZE, true);
    add_bool_trophy(game, "Beat dragon forest", "First balloon", BRONZE, true);
    add_bool_trophy(game, "Beat future fun land", "First balloon", BRONZE, true);
    add_bool_trophy_spoiler(game, "Unlocked drumstick", "Unlocked drumstick", SILVER, true);
    add_bool_trophy_spoiler(game, "Unlocked T.T.", "Unlocked T.T.", GOLD, true);
    add_bool_trophy_spoiler(game, "Beat Adventure 2", "Beat Adventure 2", GOLD, true);
}
