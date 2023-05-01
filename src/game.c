//
// Created by Shadow-Link on 21/03/2023.
//

#include <stdbool.h>

#include "game.h"
#include "trophy.h"
#include "debug.h"

void add_bool_trophy_internal(
        Game *game,
        char *title,
        char *description,
        TrophyLevel level,
        bool isCollected,
        bool containsSpoiler
) {
    printf("Adding boolean trophy at index %d - '%s'\n", game->trophyCount, title);
    if (game->trophyCount >= MAX_TROPHY_COUNT) {
        debug_print_number_and_stop("Unable to add trophy: reached max trophy count", game->trophyCount);
        return;
    }

    Trophy *trophy = &game->trophies[game->trophyCount++];
    strcpy(trophy->title, title);
    strcpy(trophy->description, description);
    trophy->level = level;
    trophy->type = BOOL;
    trophy->isCollected = isCollected;
    trophy->containsSpoilers = containsSpoiler;
}

void add_bool_trophy_spoiler(
        Game *game,
        char *title,
        char *description,
        TrophyLevel level,
        bool isCollected
) {
    add_bool_trophy_internal(game, title, description, level, isCollected, true);
}

void add_bool_trophy(
        Game *game,
        char *title,
        char *description,
        TrophyLevel level,
        bool isCollected
) {
    add_bool_trophy_internal(game, title, description, level, isCollected, false);
}

void add_counter_trophy_internal(
        Game *game,
        char *title,
        char *description,
        TrophyLevel level,
        int targetCount,
        int currentCount,
        bool containsSpoiler
) {
    printf("Adding counter trophy at index %d - '%s'\n", game->trophyCount, title);
    if (game->trophyCount >= MAX_TROPHY_COUNT) {
        debug_print_number_and_stop("Unable to add trophy: reached max trophy count", game->trophyCount);
        return;
    }

    Trophy *trophy = &game->trophies[game->trophyCount++];
    strcpy(trophy->title, title);
    strcpy(trophy->description, description);
    trophy->level = level;
    trophy->type = COUNTER;
    trophy->targetCount = targetCount;
    trophy->currentCount = currentCount;
    trophy->isCollected = currentCount == targetCount;
    trophy->containsSpoilers = containsSpoiler;
}

void add_counter_trophy(
        Game *game,
        char *title,
        char *description,
        TrophyLevel level,
        int targetCount,
        int currentCount
) {
    add_counter_trophy_internal(game, title, description, level, targetCount, currentCount, false);
}

void add_counter_trophy_spoiler(
        Game *game,
        char *title,
        char *description,
        TrophyLevel level,
        int targetCount,
        int currentCount
) {
    add_counter_trophy_internal(game, title, description, level, targetCount, currentCount, true);
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

    *percentageCompleted = ((float) completedCount / game.trophyCount * 100.0f);
}
