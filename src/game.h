//
// Created by Shadow-Link on 21/03/2023.
//

#ifndef N64_ACHIEVEMENT_COINS_GAME_H
#define N64_ACHIEVEMENT_COINS_GAME_H

#include <string.h>

#include "trophy.h"

#define MAX_TROPHY_COUNT 50

typedef struct {
    char title[50];
    Trophy trophies[MAX_TROPHY_COUNT];
    int trophyCount;
} Game;

void add_trophy(
        Game *game,
        char *title,
        char *description,
        TrophyLevel level,
        TrophyType type,
        int isCollected
);

void getGameStatus(Game game, int *bronzeCount, int *silverCount, int *goldCount, int *percentageCompleted);

#endif // N64_ACHIEVEMENT_COINS_GAME_H
