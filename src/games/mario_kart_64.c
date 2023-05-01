//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdio.h>
#include <string.h>

#include "../debug.h"
#include "../trophy.h"
#include "../game.h"
#include "../save_state_utils.h"

int get_50_cc_gold_count(char *save_data) {
    return ((save_data[0x180] & 0b11) == 0b11) +
           ((save_data[0x180] & 0b1100) == 0b1100) +
           ((save_data[0x180] & 0b110000) == 0b110000) +
           ((save_data[0x180] & 0b11000000) == 0b11000000);
}

int get_100_cc_gold_count(char *save_data) {
    return ((save_data[0x181] & 0b11) == 0b11) +
           ((save_data[0x181] & 0b1100) == 0b1100) +
           ((save_data[0x181] & 0b110000) == 0b110000) +
           ((save_data[0x181] & 0b11000000) == 0b11000000);
}

int get_150_cc_gold_count(char *save_data) {
    return ((save_data[0x182] & 0b11) == 0b11) +
           ((save_data[0x182] & 0b1100) == 0b1100) +
           ((save_data[0x182] & 0b110000) == 0b110000) +
           ((save_data[0x182] & 0b11000000) == 0b11000000);
}

int get_mirror_gold_count(char *save_data) {
    return ((save_data[0x183] & 0b11) == 0b11) +
           ((save_data[0x183] & 0b1100) == 0b1100) +
           ((save_data[0x183] & 0b110000) == 0b110000) +
           ((save_data[0x183] & 0b11000000) == 0b11000000);
}

void get_game_data_mario_kart_64(Game *game, char *save_data) {
    add_counter_trophy(game, "Slow but steady", "Beat all cups on 50CC", SILVER, 4,
                       get_50_cc_gold_count(save_data));
    add_counter_trophy(game, "Oh, It's On", "Beat all cups on 100CC", SILVER, 4,
                       get_100_cc_gold_count(save_data));
    add_counter_trophy(game, "Gotta go fast", "Beat all cups on 150CC", GOLD, 4,
                       get_150_cc_gold_count(save_data));
    add_counter_trophy(game, "Mirror on the wall who is the fastest of them all", "Beat all cups on Mirror mode",
                       GOLD, 4,
                       get_mirror_gold_count(save_data));
}
