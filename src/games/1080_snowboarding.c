//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdio.h>
#include <string.h>

#include "../save_state_utils.h"
#include "../game.h"

int get_time_millis(FILE *saveState, int address) {
    unsigned int totalValue = 0;
    unsigned char value = 0;
    fseek(saveState, address, SEEK_SET);
    fread(&value, 1, sizeof(value), saveState);
    totalValue += (value * 6000);   // Convert minutes to millis
    fread(&value, 1, sizeof(value), saveState);
    totalValue += (value * 100);    // Seconds to millis
    fread(&value, 1, sizeof(value), saveState);
    totalValue += value;            // Millis
    return totalValue;
}

char is_record_broken(FILE *saveState, int address, int record_time) {
    return get_time_millis(saveState, address) < record_time;
}


int is_any_record_broken(FILE *saveState) {
    // Checks if any of the records is broken (TODO: Might just check the isBot/Set flag?)
    return is_record_broken(saveState, 0x6D, 9000 /* 1:30:00 */) ||
           is_record_broken(saveState, 0x9D, 9000 /* 1:30:00 */) ||
           is_record_broken(saveState, 0xCD, 9000 /* 1:30:00 */) ||
           is_record_broken(saveState, 0xFD, 9000 /* 1:30:00 */) ||
           is_record_broken(saveState, 0x12D, 9000 /* 1:30:00 */) ||
           is_record_broken(saveState, 0x15D, 9000 /* 1:30:00 */);
}

int are_all_records_broken(FILE *saveState) {
    // Checks if any of the records is broken (TODO: Might just check the isBot/Set flag?)
    return is_record_broken(saveState, 0x6D, 9000 /* 1:30:00 */) &&
           is_record_broken(saveState, 0x9D, 9000 /* 1:30:00 */) &&
           is_record_broken(saveState, 0xCD, 9000 /* 1:30:00 */) &&
           is_record_broken(saveState, 0xFD, 9000 /* 1:30:00 */) &&
           is_record_broken(saveState, 0x12D, 9000 /* 1:30:00 */) &&
           is_record_broken(saveState, 0x15D, 9000 /* 1:30:00 */);
}

void get_game_data_1080(Game *game, FILE *saveState) {
    strcpy(game->title, "1080 Snowboarding");
    add_bool_trophy(game, "Into the cold", "Finish easy difficulty", BRONZE,
                    is_greater_or_equal(saveState, 0x1FA, 2));
    add_bool_trophy(game, "Powder Threat", "Finish medium difficulty", SILVER,
                    is_greater_or_equal(saveState, 0x1FA, 3));
    add_bool_trophy(game, "Stick with it", "Finish hard difficulty", SILVER,
                    is_greater_or_equal(saveState, 0x1FA, 4));
    add_bool_trophy(game, "Wit's Thicket", "Finish expert difficulty", GOLD,
                    is_greater_or_equal(saveState, 0x1FA, 5));
    add_bool_trophy(game, "Winterborn", "Beat a highscore", BRONZE, is_any_record_broken(saveState));
    add_bool_trophy(game, "Conquest", "Beat all highscores", SILVER, are_all_records_broken(saveState));
}
