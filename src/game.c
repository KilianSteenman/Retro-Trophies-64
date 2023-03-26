//
// Created by Shadow-Link on 21/03/2023.
//

#include "game.h"
#include "trophy.h"
#include "debug.h"

void add_trophy(
        Game *game,
        char *title,
        char *description,
        TrophyLevel level,
        TrophyType type,
        int isCollected
) {
    if(game->trophyCount >= MAX_TROPHY_COUNT) {
        debug_print_and_stop("Unable to add trophy: reached max trophy count");
        return;
    }

    Trophy *trophy = &game->trophies[game->trophyCount++];
    strcpy(trophy->title, title);
    strcpy(trophy->description, description);
    trophy->level = level;
    trophy->type = type;
    trophy->isCollected = isCollected;
}

void getGameStatus(Game game, int *bronzeCount, int *silverCount, int *goldCount, int *percentageCompleted) {
    int completedCount = 0;
    for (int i = 0; i < game.trophyCount; i++) {
        if (game.trophies[i].isCollected) {
            completedCount++;
            switch (game.trophies[i].level) {
                case BRONZE:
                    *bronzeCount = *bronzeCount + 1;
                    break;
                case SILVER:
                    *silverCount = *silverCount + 1;
                    break;
                case GOLD:
                    *goldCount = *goldCount + 1;
                    break;
                case PLATINUM:
                    // TODO: Implement if wanted?
                    break;
            }
        }
    }

    *percentageCompleted = ((float)completedCount / game.trophyCount * 100.0f);
}

#endif //N64_ACHIEVEMENT_COINS_GAME_H
