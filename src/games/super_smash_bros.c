//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdio.h>
#include <string.h>

#include "../debug.h"
#include "../trophy.h"
#include "../game.h"
#include "../save_state_utils.h"

typedef enum {
    MARIO,
    FOX,
    DK,
    SAMUS,
    LUIGI,
    LINK,
    YOSHI,
    CAPTAIN_FALCON,
    KIRBY,
    PIKACHU,
    JIGGLYPUFF,
    NESS
} Character;

char did_finish_break_the_targets(char *save_data, Character character) {
    return raw_is_equal(save_data, 0x470 + 0x20 * character, 10);
}

char did_finish_board_the_platforms(char *save_data, Character character) {
    return raw_is_equal(save_data, 0x478 + 0x20 * character, 10);
}

char get_unlocked_character_count(char *save_data) {
    char count = 0;
    count += raw_is_flag_set(save_data, 0x457, 0b1);
    count += raw_is_flag_set(save_data, 0x457, 0b10);
    count += raw_is_flag_set(save_data, 0x457, 0b100);
    count += raw_is_flag_set(save_data, 0x457, 0b1000);
    return count;
}

char get_targets_completed_count(char *save_data) {
    char count = 0;
    for (int i = MARIO; i <= NESS; i++) {
        if (did_finish_break_the_targets(save_data, i)) {
            count++;
        }
    }
    return count;
}

char get_platforms_completed_count(char *save_data) {
    char count = 0;
    for (int i = MARIO; i <= NESS; i++) {
        if (did_finish_board_the_platforms(save_data, i)) {
            count++;
        }
    }
    return count;
}

char get_bonus_challenges_complete(char *save_data) {
    return (get_platforms_completed_count(save_data) == 12) + (get_targets_completed_count(save_data) == 12);
}

char get_vs_matches_played(char *save_data) {
    return read_short(save_data, 0x5E0);
}

void get_game_data_super_smash_bros(Game *game, char *save_data) {
    add_bool_trophy_spoiler(game, "It's a me", "Unlock Luigi", SILVER,
                    raw_is_flag_set(save_data, 0x457, 0b1));
    add_bool_trophy_spoiler(game, "I'll take on anybody, anytime", "Unlock Captain Falcon", SILVER,
                    raw_is_flag_set(save_data, 0x457, 0b100));
    add_bool_trophy_spoiler(game, "Fuzzy pickles", "Unlock Ness", SILVER,
                    raw_is_flag_set(save_data, 0x457, 0b10));
    add_bool_trophy_spoiler(game, "Sweet dreams", "Unlock Jigglypuff", SILVER,
                    raw_is_flag_set(save_data, 0x457, 0b1000));
    add_counter_trophy(game, "Target smasher", "Complete 'Break the Targets!' with all characters", SILVER,
                       12, get_targets_completed_count(save_data));
    add_counter_trophy(game, "Jump jump jump", "Complete 'Board the Platforms!' with all characters", SILVER,
                       12, get_platforms_completed_count(save_data));
    add_counter_trophy(game, "Sound Tester", "Complete both bonus challenges", GOLD,
                       2, get_bonus_challenges_complete(save_data));
    add_counter_trophy(game, "Pokeballs only", "Unlock 'item switch' by playing 100 VS matches", GOLD,
                       100, get_vs_matches_played(save_data));
    add_counter_trophy(game, "So happy together", "Unlock all secret characters", GOLD,
                       4, get_unlocked_character_count(save_data));
}
