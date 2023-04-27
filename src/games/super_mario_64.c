//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdio.h>

#include "../save_state_utils.h"
#include "../game.h"

typedef enum {
    BOBOMB_BATTLEFIELD = 0,
    WHOMPS_FORTRESS = 1,
    JOLLY_ROGER_BAY = 2,
    COOL_COOL_MOUNTAIN = 3,
    BIG_BOOS_HUNT = 4,
    HAZY_MAZE_CAVE = 5,
    LETHAL_LAVA_LAND = 6,
    SHIFTING_SAND_LAND = 7,
    DIRE_DIRE_DOCKS = 8,
    SNOWMANS_LAND = 9,
    WET_DRY_LAND = 10,
    TALL_TALL_MOUNTAIN = 11,
    TINY_HUGE_ISLAND = 12,
    TICK_TOCK_CLOCK = 13,
    RAINBOW_RIDE = 14,
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

int get_star_count_for_address(char *save_data, int address) {
    int map_flags = save_data[address];
    return ((map_flags & 0b1) == 0b1) +
           ((map_flags & 0b10) == 0b10) +
           ((map_flags & 0b100) == 0b100) +
           ((map_flags & 0b1000) == 0b1000) +
           ((map_flags & 0b10000) == 0b10000) +
           ((map_flags & 0b100000) == 0b100000) +
           ((map_flags & 0b1000000) == 0b1000000);
}

int get_star_count_for_map(char *save_data, Map map) {
    return get_star_count_for_address(save_data, 12 + map);
}

int get_star_count(char *save_data) {
    int star_count = 0;
    star_count += get_star_count_for_address(save_data, 0x8);
    for (int i = BOBOMB_BATTLEFIELD; i <= RAINBOW_RIDE; i++) {
        star_count += get_star_count_for_map(save_data, i);
    }
    star_count += get_star_count_for_address(save_data, 0x1B);
    star_count += get_star_count_for_address(save_data, 0x1C);
    star_count += get_star_count_for_address(save_data, 0x1D);
    star_count += get_star_count_for_address(save_data, 0x1E);
    star_count += get_star_count_for_address(save_data, 0x1F);
    star_count += get_star_count_for_address(save_data, 0x20);
    star_count += get_star_count_for_address(save_data, 0x21);
    star_count += get_star_count_for_address(save_data, 0x22);
    star_count += get_star_count_for_address(save_data, 0x23);
    return star_count;
}

char is_wing_cap_switch_activated(char *save_data) {
    return raw_is_flag_set(save_data, 0xB, 0b10);
}

char is_metal_cap_switch_activated(char *save_data) {
    return raw_is_flag_set(save_data, 0xB, 0b100);
}

char is_vanish_cap_switch_activated(char *save_data) {
    return raw_is_flag_set(save_data, 0xB, 0b1000);
}

char is_basement_unlocked(char *save_data) {
    return raw_is_flag_set(save_data, 0xB, 0b1000000);
}

char is_second_floor_unlocked(char *save_data) {
    return raw_is_flag_set(save_data, 0xB, 0b10000000);
}

char is_third_floor_unlocked(char *save_data) {
    return get_star_count(save_data) >= 50;
}

char is_moat_drained(char *save_data) {
    return raw_is_flag_set(save_data, 0xA, 0b10);
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
    add_counter_trophy(game, "Whompty Thwompty", "Collect all stars on Whomp's fortress", SILVER,
                       7, get_star_count_for_map(save_data, WHOMPS_FORTRESS));
    add_counter_trophy(game, "Mayday", "Collect all stars on Jolly Roger Bay", SILVER,
                       7, get_star_count_for_map(save_data, JOLLY_ROGER_BAY));
    add_counter_trophy(game, "Let it snow", "Collect all stars on Cool, Cool Mountain", SILVER,
                       7, get_star_count_for_map(save_data, COOL_COOL_MOUNTAIN));
    add_counter_trophy(game, "Who needs a poltergust?", "Collect all stars on Big Boos Hunt", SILVER,
                       7, get_star_count_for_map(save_data, BIG_BOOS_HUNT));
    add_counter_trophy(game, "Lets go underground", "Collect all stars on Hazy Maze Cave", SILVER,
                       7, get_star_count_for_map(save_data, HAZY_MAZE_CAVE));
    add_counter_trophy(game, "It's getting hot in here", "Collect all stars on Lethal Lava Land", SILVER,
                       7, get_star_count_for_map(save_data, LETHAL_LAVA_LAND));
    add_counter_trophy(game, "Walking the Desert!", "Collect all stars on Shifting sand land", SILVER,
                       7, get_star_count_for_map(save_data, SHIFTING_SAND_LAND));
    add_counter_trophy(game, "Ship ahoy", "Collect all stars on Dire Dire Docks", SILVER,
                       7, get_star_count_for_map(save_data, DIRE_DIRE_DOCKS));
    add_counter_trophy(game, "Frosty the snowman", "Collect all stars on Snowman's land", SILVER,
                       7, get_star_count_for_map(save_data, SNOWMANS_LAND));
    add_counter_trophy(game, "Soaked", "Collect all stars on Wet Dry World", SILVER,
                       7, get_star_count_for_map(save_data, WET_DRY_LAND));
    add_counter_trophy(game, "No fear of heights", "Collect all stars on Tall Tall Mountain", SILVER,
                       7, get_star_count_for_map(save_data, TALL_TALL_MOUNTAIN));
    add_counter_trophy(game, "It's a small world after all", "Collect all stars on Tiny Huge Island", SILVER,
                       7, get_star_count_for_map(save_data, TINY_HUGE_ISLAND));
    add_counter_trophy(game, "Time traveler", "Collect all stars on Tick Tock Clock", SILVER,
                       7, get_star_count_for_map(save_data, TICK_TOCK_CLOCK));
    add_counter_trophy(game, "Somewhere over the rainbow", "Collect all stars on Rainbow Ride", SILVER,
                       7, get_star_count_for_map(save_data, RAINBOW_RIDE));
    add_bool_trophy(game, "Come fly with me", "Activate the wing cap switch", SILVER,
                    is_wing_cap_switch_activated(save_data));
    add_bool_trophy(game, "Heavy hitter", "Activate the metal cap switch", SILVER,
                    is_metal_cap_switch_activated(save_data));
    add_bool_trophy(game, "Where did you go?", "Activate the vanish cap switch", SILVER,
                    is_vanish_cap_switch_activated(save_data));
    add_bool_trophy(game, "I hate basements", "Get access to the basement", GOLD,
                    is_basement_unlocked(save_data));
    add_bool_trophy(game, "The higher the better", "Get access to the second floor", GOLD,
                    is_second_floor_unlocked(save_data));
    add_bool_trophy(game, "Up up up", "Get access to the third floor", GOLD,
                    is_third_floor_unlocked(save_data));
    add_bool_trophy(game, "Down the drain", "Drain the moat", SILVER,
                    is_moat_drained(save_data));
    add_counter_trophy(game, "It's all about the money", "Collect all coin stars", GOLD,
                       15, get_coin_star_count(save_data));
    add_counter_trophy(game, "Stargazer", "Collect all stars", GOLD,
                       120, get_star_count(save_data));
}
