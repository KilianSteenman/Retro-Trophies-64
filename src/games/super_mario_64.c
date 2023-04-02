//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdio.h>

#include "../save_state_utils.h"
#include "../game.h"

typedef enum {
    BOBOMB_BATTLEFIELD = 0
} Map;

int get_coin_star_count(char *save_data) {
    // There are 15 coin counters starting at offset 0x25
    int coinStarsCollected = 0;
    char coinCount;
    for (int i = 0; i < 15; i++) {
        coinCount = save_data[0x25 + i];
        if (coinCount >= 100) {
            coinStarsCollected++;
        }
    }

    return coinStarsCollected;
}

int get_star_count_for_map(char *save_data, Map map) {
    int map_flags = save_data[12 + map];
    return ((map_flags & 0b1) == 0b1) +
           ((map_flags & 0b10) == 0b10) +
           ((map_flags & 0b100) == 0b100) +
           ((map_flags & 0b1000) == 0b1000) +
           ((map_flags & 0b10000) == 0b10000) +
           ((map_flags & 0b100000) == 0b100000) +
           ((map_flags & 0b1000000) == 0b1000000);
}

void get_game_data_mario64(Game *game, FILE *saveState) {
    char save_data[512];
    if (fread(&save_data, sizeof(save_data), 1, saveState)) {
        printf("Read mario save game into memory\n");
    }

    add_bool_trophy(game, "Welcome to the third dimension", "Collect your first star", BRONZE,
                    raw_is_greater_or_equal(save_data, 0xC, 1));
    add_counter_trophy(game, "Bob-omb boom!", "Collect all stars on Bob-omb battlefield", SILVER,
                    7, get_star_count_for_map(save_data, BOBOMB_BATTLEFIELD));
    add_counter_trophy(game, "It's all about the money", "Collect all coin stars", SILVER,
                       15, get_coin_star_count(save_data));
}
