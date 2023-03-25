//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../game.h"

void greaterThenOrEqual(
        FILE *saveState,
        Trophy *trophy,
        char *title,
        char *description,
        TrophyLevel level,
        int address,
        int requiredValue
) {
    strcpy(trophy->title, title);
    strcpy(trophy->description, description);
    trophy->level = level;
    trophy->type = BOOL;

    fseek(saveState, address, SEEK_SET);
    unsigned short value = 0;
    fread(&value, 1, sizeof(value), saveState);
    printf("Value %d", value);//: %d", value);
    while (1) {}

    if (value >= requiredValue) {
        trophy->isCollected = 1;
    } else {
        trophy->isCollected = 0;
    }
}

void getGameData1080(Game *game, FILE *saveState) {
    strcpy(game->title, "1080 Snowboarding");

    game->trophyCount = 4;
    game->trophies = (Trophy *) malloc(sizeof(Trophy) * game->trophyCount);
    greaterThenOrEqual(saveState, &game->trophies[0], "Into the cold", "Finish easy difficulty", BONUS, 0x1FB, 2);
    greaterThenOrEqual(saveState, &game->trophies[1], "Powder Threat", "Finish medium difficulty", BONUS, 0x1FB, 3);
    greaterThenOrEqual(saveState, &game->trophies[2], "Stick with it", "Finish hard difficulty", BONUS, 0x1FB, 4);
    greaterThenOrEqual(saveState, &game->trophies[3], "Wit's Thicket", "Finish expert difficulty", BONUS, 0x1FB, 5);
}
