//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdio.h>
#include <string.h>

#include "game.h"

int is_greater_or_equal_mario64(FILE *saveState, int address, int requiredValue) {
    unsigned short value = 0;
    fseek(saveState, address, SEEK_SET);
    fread(&value, 1, sizeof(value), saveState);
    return value >= requiredValue;
}

int is_trophy_collected_money_money_money(FILE *saveState) {
    fseek(saveState, 0x25, SEEK_SET);
    int coinStarsCollected = 0;
    char coinCount;
    for(int i = 0x25; i <= 0x33; i++) {
        fread(&coinCount, 1, sizeof(coinCount), saveState);
        if(coinCount >= 100) {
            coinStarsCollected++;
        }
    }
    return coinStarsCollected == 0xE;
}

void get_game_data_mario64(Game *game, FILE *saveState) {
    strcpy(game->title, "Super Mario 64");
    add_trophy(game, "Welcome to the third dimension", "Collect your first star", BRONZE, BOOL,
               is_greater_or_equal_mario64(saveState, 0xC, 1));
    add_trophy(game, "Bob-omb boom!", "Collect all stars on Bob-omb battlefield", SILVER, COUNTER,
               is_greater_or_equal_mario64(saveState, 0xC, 0b11111110));
    add_trophy(game, "It's all about the money", "Collect all coin stars", SILVER, BOOL,
               is_trophy_collected_money_money_money(saveState));
}