//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdio.h>
#include <string.h>

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

char did_finish_break_the_targets(FILE *saveState, Character character) {
    return is_equal(saveState, 0x470 + 0x20 * character, 10);
}

char get_unlocked_character_count(FILE *saveState) {
    char count = 0;
    count += is_flag_set(saveState, 0x457, 0b1);
    count += is_flag_set(saveState, 0x457, 0b10);
    count += is_flag_set(saveState, 0x457, 0b100);
    count += is_flag_set(saveState, 0x457, 0b1000);
    return count;
}

char get_targets_completed_count(FILE *saveState) {
    char count = 0;
    for (int i = MARIO; i <= NESS; i++) {
        if (did_finish_break_the_targets(saveState, i)) {
            count++;
        }
    }
    return count;
}

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
    add_counter_trophy(game, "Target smasher", "Complete 'Break the Targets!' with all characters", GOLD,
                       12, get_targets_completed_count(saveState));
    add_counter_trophy(game, "So happy together", "Unlock all secret characters", GOLD,
                       4, get_unlocked_character_count(saveState));
}
