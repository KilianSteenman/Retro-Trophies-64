//
// Created by Shadow-Link on 21/03/2023.
//

#include "trophy.h"

#ifndef N64_ACHIEVEMENT_COINS_GAME_H
#define N64_ACHIEVEMENT_COINS_GAME_H

typedef struct {
    char title[50];
    Trophy *trophies;
    int trophyCount;
} Game;

void getGameStatus(Game game, int *bronzeCount, int *silverCount, int *goldCount, int *percentageCompleted) {
    *bronzeCount = 10;
    *silverCount = 3;
    *goldCount = 1;
    *percentageCompleted = 27;
}

#endif //N64_ACHIEVEMENT_COINS_GAME_H
