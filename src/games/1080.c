//
// Created by Shadow-Link on 25/03/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../game.h"
#include "../debug.c"

int trophyIndex = 0;

int isGreaterOrEqual(FILE *saveState, int address, int requiredValue) {
    unsigned short value = 0;
    fseek(saveState, address, SEEK_SET);
    fread(&value, 1, sizeof(value), saveState);
    return value >= requiredValue;
}

void addTrophy(
        Game *game,
        char *title,
        char *description,
        TrophyLevel level,
        TrophyType type,
        int isCollected
) {
    game->trophyCount = game->trophyCount + 1;
    Trophy *trophy = &game->trophies[trophyIndex++];
    strcpy(trophy->title, title);
    strcpy(trophy->description, description);
    trophy->level = level;
    trophy->type = type;
    trophy->isCollected = isCollected;
}

int winterborn(FILE *saveState) {
    unsigned int totalValue = 0;
    unsigned char value = 0;
    fseek(saveState, 0x6D, SEEK_SET);
    fread(&value, 1, sizeof(value), saveState);
    totalValue += (value * 600);
    fread(&value, 1, sizeof(value), saveState);
    totalValue += (value * 100);
    fread(&value, 1, sizeof(value), saveState);
    totalValue += value;

    return totalValue < 9000;
}

void getGameData1080(Game *game, FILE *saveState) {
    trophyIndex = 0;

    strcpy(game->title, "1080 Snowboarding");
    addTrophy(game, "Into the cold", "Finish easy difficulty", BONUS, BOOL,
              isGreaterOrEqual(saveState, 0x1FA, 2));
    addTrophy(game, "Powder Threat", "Finish medium difficulty", BONUS, BOOL,
              isGreaterOrEqual(saveState, 0x1FA, 3));
    addTrophy(game, "Stick with it", "Finish hard difficulty", BONUS, BOOL,
              isGreaterOrEqual(saveState, 0x1FA, 4));
    addTrophy(game, "Wit's Thicket", "Finish expert difficulty", BONUS, BOOL,
              isGreaterOrEqual(saveState, 0x1FA, 5));
    addTrophy(game, "Winterborn", "Beat the highscore", BONUS, BOOL, winterborn(saveState));
}
