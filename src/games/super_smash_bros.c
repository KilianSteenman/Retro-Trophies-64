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

char did_finish_break_the_targets(char *save_data, Character character) {
    return raw_is_equal(save_data, 0x470 + 0x20 * character, 10);
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

void get_game_data_super_smash_bros(Game *game, FILE *saveState) {
    char save_data[32768];
    if (fread(&save_data, sizeof(save_data), 1, saveState)) {
        printf("Read super smash bros save game into memory\n");
    }

    add_bool_trophy(game, "It's a me", "Unlock Luigi", SILVER,
                    raw_is_flag_set(save_data, 0x457, 0b1));
    add_bool_trophy(game, "I'll take on anybody, anytime", "Unlock Captain Falcon", SILVER,
                    raw_is_flag_set(save_data, 0x457, 0b100));
    add_bool_trophy(game, "Fuzzy pickles", "Unlock Ness", SILVER,
                    raw_is_flag_set(save_data, 0x457, 0b10));
    add_bool_trophy(game, "Sweet dreams", "Unlock Jigglypuff", SILVER,
                    raw_is_flag_set(save_data, 0x457, 0b1000));
    add_counter_trophy(game, "Target smasher", "Complete 'Break the Targets!' with all characters", GOLD,
                       12, get_targets_completed_count(save_data));
    add_counter_trophy(game, "So happy together", "Unlock all secret characters", GOLD,
                       4, get_unlocked_character_count(save_data));
}
