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
    int completedCount = 0;
    for (int i = 0; i < game.trophyCount; i++) {
        if (game.trophies[i].isCollected) {
            completedCount++;
            switch (game.trophies[i].level) {
                case BONUS:
                    *bronzeCount = *bronzeCount + 1;
                    break;
                case MILESTONE:
                    *silverCount = *silverCount + 1;
                    break;
                case FINISHED:
                    *goldCount = *goldCount + 1;
                    break;
                case COMPLETED:
                    // TODO: Implement if wanted?
                    break;
            }
        }
    }

    *percentageCompleted = ((float)completedCount / game.trophyCount * 100.0f);
}

#endif //N64_ACHIEVEMENT_COINS_GAME_H
