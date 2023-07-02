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

bool has_beaten_car_challenge(char *save_data) {
    return raw_is_flag_set(save_data, 0x32, 0b10000000);
}

bool has_beaten_hovercraft_challenge(char *save_data) {
    return raw_is_flag_set(save_data, 0x33, 0b00000001);
}

bool has_beaten_plane_challenge(char *save_data) {
    return raw_is_flag_set(save_data, 0x33, 0b00000010);
}

bool has_beaten_all_challenges(char *save_data) {
    return has_beaten_car_challenge(save_data) &&
           has_beaten_hovercraft_challenge(save_data) &&
           has_beaten_plane_challenge(save_data);
}

bool has_first_balloon(char *save_data) {
    return get_total_balloon_count(save_data) >= 1;
}

int get_golden_trophy_count(char *save_data) {
    char trophy_1 = (read_byte(save_data, 0x33) >> 2 & 0x3);// == 0x3;
    char trophy_2 = (read_byte(save_data, 0x33) >> 4 & 0x3);// == 0x3;
    char trophy_3 = (read_byte(save_data, 0x33) >> 6 & 0x3);// == 0x3;
    char trophy_4 = (read_byte(save_data, 0x34) & 0x3);// == 0x3;
    char trophy_5 = (read_byte(save_data, 0x34) >> 2 & 0x3);// == 0x3;
    char buffer[20];
    sprintf(buffer, "Trophy %d\n", trophy_1);
    debug_printf(buffer);
    sprintf(buffer, "Trophy %d\n", trophy_2);
    debug_printf(buffer);
    sprintf(buffer, "Trophy %d\n", trophy_3);
    debug_printf(buffer);
    sprintf(buffer, "Trophy %d\n", trophy_4);
    debug_printf(buffer);
    sprintf(buffer, "Trophy %d\n", trophy_5);
    debug_printf(buffer);
    debug_printf_and_pause("DKR Trophies: %d %d %d %d %d\n", trophy_1, trophy_2, trophy_3, trophy_4, trophy_5);
    // TODO: Read save data
    return trophy_1 + trophy_2 + trophy_3 + trophy_4 + trophy_5;
}

void get_game_data_diddy_kong_racing(Game *game, char *save_data) {
    add_bool_trophy(game, "First balloon", "Collect a golden balloon", BRONZE, has_first_balloon(save_data));
    add_counter_trophy(game, "All balloons", "Collect all golden balloons", BRONZE, 47,
                       get_total_balloon_count(save_data));
    add_bool_trophy(game, "Car challenge", "Car challenge", BRONZE, has_beaten_car_challenge(save_data));
    add_bool_trophy(game, "Hovercraft challenge", "Hovercraft challenge", BRONZE,
                    has_beaten_hovercraft_challenge(save_data));
    add_bool_trophy(game, "Plane challenge", "Plane challenge", BRONZE, has_beaten_plane_challenge(save_data));
    add_bool_trophy(game, "Beat all Taj Challenges", "Beat all Taj Challenges", SILVER, has_beaten_all_challenges(save_data));
    add_counter_trophy(game, "Beat all levels", "Beat all levels", BRONZE, 50, 0);
    add_counter_trophy(game, "Beat all silver coin", "Beat all silver coin", BRONZE, 50, 0);
    add_counter_trophy(game, "Got all golden trophies", "Got all golden trophies", GOLD, 5, get_golden_trophy_count(save_data));
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
