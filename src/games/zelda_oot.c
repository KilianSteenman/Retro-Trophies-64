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

bool has_all_magic_upgrades(char *save_data) {
    return save_data[0x52] == 2;
}

bool has_found_all_maps_and_compasses(char *save_data) {
    // TODO: read all dungeon item states
    return false;
}

int get_learned_song_count(char *save_data) {
    return raw_is_flag_set(save_data, 0x0C5, 0b00000010) +     // Song of storms
           raw_is_flag_set(save_data, 0x0C5, 0b00000001) +    // Song of time
           raw_is_flag_set(save_data, 0x0C6, 0b10000000) +    // Sung song
           raw_is_flag_set(save_data, 0x0C6, 0b01000000) +    // Sarias song
           raw_is_flag_set(save_data, 0x0C6, 0b00100000) +    // Eponas song
           raw_is_flag_set(save_data, 0x0C6, 0b00010000) +    // Zeldas lullaby
           raw_is_flag_set(save_data, 0x0C6, 0b00001000) +    // Prelude of light
           raw_is_flag_set(save_data, 0x0C6, 0b00000100) +    // Nocturne Of Shadow
           raw_is_flag_set(save_data, 0x0C6, 0b00000010) +    // Requiem Of Spirit
           raw_is_flag_set(save_data, 0x0C6, 0b00000001) +    // Serenade Of Water
           raw_is_flag_set(save_data, 0x0C7, 0b10000000) +    // Bolero Of Fire
           raw_is_flag_set(save_data, 0x0C6, 0b01000000);      // Minuet Of Forest
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

    add_bool_trophy(game, "The flow of time is always cruel...", "Obtain the Forest Medallion", SILVER,
                    raw_is_flag_set(save_data, 0x0C7, 0b00000001));
    add_bool_trophy(game, "It is something that grows over time...", "Obtain the Fire Medallion", SILVER,
                    raw_is_flag_set(save_data, 0x0C7, 0b00000010));
    add_bool_trophy(game, "Time passes, people move...", "Obtain the Water Medallion", SILVER,
                    raw_is_flag_set(save_data, 0x0C7, 0b00000100));
    add_bool_trophy(game, "Past, present, future...", "Obtain the Spirit Medallion", SILVER,
                    raw_is_flag_set(save_data, 0x0C7, 0b00001000));
    add_bool_trophy(game, "The infinite darkness that absorbs even time...", "Obtain the Shadow Medallion", SILVER,
                    raw_is_flag_set(save_data, 0x0C7, 0b00010000));

    add_bool_trophy(game, "The silent cartographer", "Collected all maps and compasses", GOLD,
                    has_found_all_maps_and_compasses(save_data));
    add_counter_trophy(game, "I'll never forget this song!", "Learned all songs", GOLD,
                       12, get_learned_song_count(save_data));

    add_bool_trophy(game, "Creepy little buggers", "Collect your first Gold Skulltula", BRONZE,
                    raw_is_flag_set(save_data, 0x0C5, 0b10000000));
    add_bool_trophy(game, "You're a wizard Link", "Obtain all magic upgrades", GOLD,
                    has_all_magic_upgrades(save_data));
    add_counter_trophy(game, "At least it's not 900", "Collect all Gold Skulltulas", GOLD,
                       100, get_skulltula_count(save_data));
}
