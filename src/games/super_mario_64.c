//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdio.h>
#include <string.h>

#include "../debug.h"
#include "../save_state_utils.h"
#include "../game.h"

int are_all_coin_stars_collected(FILE *saveState) {
    fseek(saveState, 0x25, SEEK_SET);
    int coinStarsCollected = 0;
    char coinCount;
    for (int i = 0x25; i <= 0x33; i++) {
        fread(&coinCount, 1, sizeof(coinCount), saveState);
        if (coinCount >= 100) {
            coinStarsCollected++;
        }
    }

    return coinStarsCollected;
}

void get_game_data_mario64(Game *game, FILE *saveState) {
    strcpy(game->title, "Super Mario 64");
    add_bool_trophy(game, "Welcome to the third dimension", "Collect your first star", BRONZE,
                    is_greater_or_equal(saveState, 0xC, 1));
    add_bool_trophy(game, "Bob-omb boom!", "Collect all stars on Bob-omb battlefield", SILVER,
                    is_greater_or_equal(saveState, 0xC, 0b11111110));
    add_counter_trophy(game, "It's all about the money", "Collect all coin stars", SILVER,
                       15, are_all_coin_stars_collected(saveState));
}