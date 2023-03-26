//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdio.h>
#include <string.h>

#include "../game.h"

int is_greater_or_equal_1080(FILE *saveState, int address, int requiredValue) {
    unsigned short value = 0;
    fseek(saveState, address, SEEK_SET);
    fread(&value, 1, sizeof(value), saveState);
    return value >= requiredValue;
}

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

int is_trophy_collected_winterborn(FILE *saveState) {
    return get_time_millis(saveState, 0x6D) < 9000; // 1:30:00
}

void get_game_data_1080(Game *game, FILE *saveState) {
    strcpy(game->title, "1080 Snowboarding");
    add_trophy(game, "Into the cold", "Finish easy difficulty", BRONZE, BOOL,
               is_greater_or_equal_1080(saveState, 0x1FA, 2));
    add_trophy(game, "Powder Threat", "Finish medium difficulty", SILVER, BOOL,
               is_greater_or_equal_1080(saveState, 0x1FA, 3));
    add_trophy(game, "Stick with it", "Finish hard difficulty", SILVER, BOOL,
               is_greater_or_equal_1080(saveState, 0x1FA, 4));
    add_trophy(game, "Wit's Thicket", "Finish expert difficulty", GOLD, BOOL,
               is_greater_or_equal_1080(saveState, 0x1FA, 5));
    add_trophy(game, "Winterborn", "Beat the highscore", BRONZE, BOOL, is_trophy_collected_winterborn(saveState));
}
