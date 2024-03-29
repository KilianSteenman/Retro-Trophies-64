//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdio.h>

#include "../debug.h"
#include "../save_state_utils.h"
#include "../game.h"

typedef enum {
    EASY = 1,
    MEDIUM = 2,
    HARD = 3,
    EXPERT = 4
} Difficulty;

int get_time_millis(char *save_data, int address) {
    unsigned int totalValue = 0;
    totalValue += (save_data[address] * 6000);      // Convert minutes to millis
    totalValue += (save_data[address + 1] * 100);   // Seconds to millis
    totalValue += save_data[address + 2];           // Millis
    return totalValue;
}

char is_time_trial_record_broken(char *save_data, int address, int minutes, int seconds, int millis) {
    int time_to_beat = (minutes * 6000) + (seconds * 100) + millis;
    int record_time = get_time_millis(save_data, address);
    char buffer[500];
    sprintf(buffer, "time to beat: %d - %d\n", time_to_beat, record_time);
    debug_printf_and_pause(buffer);
    return record_time < time_to_beat;
}

int time_trial_record_count(char *save_data) {
    return is_time_trial_record_broken(save_data, 0x6D, 1, 30, 0) +
           is_time_trial_record_broken(save_data, 0x9D, 2, 0, 0) +
           is_time_trial_record_broken(save_data, 0xCD, 2, 0, 0) +
           is_time_trial_record_broken(save_data, 0xFD, 2, 30, 0) +
           is_time_trial_record_broken(save_data, 0x12D, 2, 30, 0) +
           is_time_trial_record_broken(save_data, 0x15D, 2, 0, 0);
}

int is_any_time_trial_record_broken(char *save_data) {
    return time_trial_record_count(save_data) >= 1;
}

char has_beaten_difficulty(char *save_data, Difficulty difficulty) {
    return read_short(save_data, 0x1FA) > difficulty;
}

void get_game_data_1080(Game *game, char *save_data) {
    add_bool_trophy(game, "Into the cold", "Finish easy difficulty", BRONZE,
                    has_beaten_difficulty(save_data, EASY));
    add_bool_trophy(game, "Powder Threat", "Finish medium difficulty", SILVER,
                    has_beaten_difficulty(save_data, MEDIUM));
    add_bool_trophy(game, "Stick with it", "Finish hard difficulty", SILVER,
                    has_beaten_difficulty(save_data, HARD));
    add_bool_trophy(game, "Wit's Thicket", "Finish expert difficulty", GOLD,
                    has_beaten_difficulty(save_data, EXPERT));
    add_bool_trophy(game, "Winterborn", "Beat a time highscore", BRONZE, is_any_time_trial_record_broken(save_data));
    add_counter_trophy(game, "Conquest", "Beat all time highscores", SILVER, 6, time_trial_record_count(save_data));
}
