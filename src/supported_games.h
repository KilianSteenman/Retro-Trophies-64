//
// Created by Shadow-Link on 06/05/2023.
//

#ifndef N64_TROPHIES_SUPPORTED_GAMES_H
#define N64_TROPHIES_SUPPORTED_GAMES_H

#include "region.h"
#include "save_type.h"
#include "games/1080_snowboarding.h"
#include "games/super_mario_64.h"
#include "games/super_smash_bros.h"
#include "games/mario_kart_64.h"
#include "games/zelda_oot.h"

#define SUPPORTED_GAME_COUNT 10

typedef void (*TrophyDataLoader)(Game *, char *);

typedef struct {
    char name[50];
    char game_code[4];
    SaveType save_type;
    Region region;
    TrophyDataLoader trophy_data_loader;
} SupportedGame;

SupportedGame supported_games[SUPPORTED_GAME_COUNT];

void init_supported_games() {
    supported_games[0] = (SupportedGame) {.name = "1080 Snowboarding", .game_code = "TEA", .region = USA, .save_type = RAM, .trophy_data_loader = get_game_data_1080};
    supported_games[1] = (SupportedGame) {.name = "1080 Snowboarding", .game_code = "TEP", .region = PAL, .save_type = RAM, .trophy_data_loader = get_game_data_1080};
    supported_games[2] = (SupportedGame) {.name = "Super Smash Bros", .game_code = "ALE", .region = USA, .save_type = RAM, .trophy_data_loader = get_game_data_super_smash_bros};
    supported_games[3] = (SupportedGame) {.name = "Super Smash Bros", .game_code = "ALP", .region = PAL, .save_type = RAM, .trophy_data_loader = get_game_data_super_smash_bros};
    supported_games[4] = (SupportedGame) {.name = "Super Mario 64", .game_code = "SME", .region = USA, .save_type = EEP, .trophy_data_loader = get_game_data_mario64};
    supported_games[5] = (SupportedGame) {.name = "Super Mario 64", .game_code = "SMP", .region = PAL, .save_type = EEP, .trophy_data_loader = get_game_data_mario64};
    supported_games[6] = (SupportedGame) {.name = "Mario Kart 64", .game_code = "KTE", .region = USA, .save_type = EEP, .trophy_data_loader = get_game_data_mario_kart_64};
    supported_games[7] = (SupportedGame) {.name = "Mario Kart 64", .game_code = "KTP", .region = PAL, .save_type = EEP, .trophy_data_loader = get_game_data_mario_kart_64};
    supported_games[8] = (SupportedGame) {.name = "The Legend of Zelda: Ocarina of Time", .game_code = "ZLE", .region = USA, .save_type = RAM, .trophy_data_loader = get_trophy_data_zelda_oot};
    supported_games[9] = (SupportedGame) {.name = "The Legend of Zelda: Ocarina of Time", .game_code = "ZLP", .region = PAL, .save_type = RAM, .trophy_data_loader = get_trophy_data_zelda_oot};
}

#endif //N64_TROPHIES_SUPPORTED_GAMES_H
