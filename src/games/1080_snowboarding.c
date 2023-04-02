//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdio.h>

#include "../save_state_utils.h"
#include "../game.h"

int get_time_millis(char *save_data, int address) {
    unsigned int totalValue = 0;
    totalValue += (save_data[address] * 6000);      // Convert minutes to millis
    totalValue += (save_data[address + 1] * 100);   // Seconds to millis
    totalValue += save_data[address + 1];           // Millis
    return totalValue;
}

char is_record_broken(char *save_data, int address, int record_time) {
    return get_time_millis(save_data, address) < record_time;
}


int is_any_record_broken(char *save_data) {
    // Checks if any of the records is broken (TODO: Might just check the isBot/Set flag?)
    return is_record_broken(save_data, 0x6D, 9000 /* 1:30:00 */) ||
           is_record_broken(save_data, 0x9D, 9000 /* 1:30:00 */) ||
           is_record_broken(save_data, 0xCD, 9000 /* 1:30:00 */) ||
           is_record_broken(save_data, 0xFD, 9000 /* 1:30:00 */) ||
           is_record_broken(save_data, 0x12D, 9000 /* 1:30:00 */) ||
           is_record_broken(save_data, 0x15D, 9000 /* 1:30:00 */);
}

int record_time_count(char *save_data) {
    return is_record_broken(save_data, 0x6D, 9000 /* 1:30:00 */) +
           is_record_broken(save_data, 0x9D, 9000 /* 1:30:00 */) +
           is_record_broken(save_data, 0xCD, 9000 /* 1:30:00 */) +
           is_record_broken(save_data, 0xFD, 9000 /* 1:30:00 */) +
           is_record_broken(save_data, 0x12D, 9000 /* 1:30:00 */) +
           is_record_broken(save_data, 0x15D, 9000 /* 1:30:00 */);
}

void get_game_data_1080(Game *game, FILE *saveState) {
    char save_data[32768];
    if (fread(&save_data, sizeof(save_data), 1, saveState)) {
        printf("Read 1080 save game into memory\n");
    }

    add_bool_trophy(game, "Into the cold", "Finish easy difficulty", BRONZE,
                    raw_is_greater_or_equal(save_data, 0x1FA, 2));
    add_bool_trophy(game, "Powder Threat", "Finish medium difficulty", SILVER,
                    raw_is_greater_or_equal(save_data, 0x1FA, 3));
    add_bool_trophy(game, "Stick with it", "Finish hard difficulty", SILVER,
                    raw_is_greater_or_equal(save_data, 0x1FA, 4));
    add_bool_trophy(game, "Wit's Thicket", "Finish expert difficulty", GOLD,
                    raw_is_greater_or_equal(save_data, 0x1FA, 5));
    add_bool_trophy(game, "Winterborn", "Beat a highscore", BRONZE, is_any_record_broken(save_data));
    add_counter_trophy(game, "Conquest", "Beat all highscores", SILVER, 6, record_time_count(save_data));
}
