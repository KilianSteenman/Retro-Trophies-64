//
// Created by Shadow-Link on 21/03/2023.
//

#ifndef N64_ACHIEVEMENT_COINS_GAME_H
#define N64_ACHIEVEMENT_COINS_GAME_H

#include <stdbool.h>
#include <string.h>

#include "region.h"
#include "trophy.h"

#define MAX_TROPHY_COUNT 50

typedef struct {
    char title[50];
    char filename[100];
    Region region;
    Trophy trophies[MAX_TROPHY_COUNT];
    int trophyCount;
} Game;

void add_bool_trophy(
        Game *game,
        char *title,
        char *description,
        TrophyLevel level,
        bool isCollected
);

void add_bool_trophy_spoiler(
        Game *game,
        char *title,
        char *description,
        TrophyLevel level,
        bool isCollected
);

void add_counter_trophy(
        Game *game,
        char *title,
        char *description,
        TrophyLevel level,
        int targetCount,
        int currentCount
);

void add_counter_trophy_spoiler(
        Game *game,
        char *title,
        char *description,
        TrophyLevel level,
        int targetCount,
        int currentCount
);

void getGameStatus(Game game, int *bronzeCount, int *silverCount, int *goldCount, int *percentageCompleted);

#endif // N64_ACHIEVEMENT_COINS_GAME_H
