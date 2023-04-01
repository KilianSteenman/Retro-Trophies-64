//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdio.h>
#include <string.h>

#include "../debug.h"
#include "../save_state_utils.h"
#include "../game.h"

int get_coin_star_count(char *save_data) {
    // There are 15 coin counts starting at offset 0x25
//    fseek(saveState, 0x25, SEEK_SET);
    int coinStarsCollected = 0;
    char coinCount;
    for (int i = 0; i < 15; i++) {
        coinCount = save_data[0x25 + i];
//        fread(&coinCount, 1, sizeof(coinCount), saveState);
        if (coinCount >= 100) {
            coinStarsCollected++;
        }
    }

    return coinStarsCollected;
}

int raw_is_greater_or_equal(char *save_data, int address, int requiredValue) {
//    unsigned short value = 0;
//    fseek(saveState, address, SEEK_SET);
//    fread(&value, 1, sizeof(value), saveState);
    return save_data[address] >= requiredValue;
}

void get_game_data_mario64(Game *game, FILE *saveState) {
    char save_data[512];
    if(fread(&save_data, sizeof(save_data), 1, saveState)) {
        printf("Read mario save game into memory\n");
    }

    strcpy(game->title, "Super Mario 64");
    game->trophyCount = 0;
    add_bool_trophy(game, "Welcome to the third dimension", "Collect your first star", BRONZE,
                    raw_is_greater_or_equal(save_data, 0xC, 1));
    add_bool_trophy(game, "Bob-omb boom!", "Collect all stars on Bob-omb battlefield", SILVER,
                    raw_is_greater_or_equal(save_data, 0xC, 0b11111110));
    add_counter_trophy(game, "It's all about the money", "Collect all coin stars", SILVER,
                       15, get_coin_star_count(save_data));
}
