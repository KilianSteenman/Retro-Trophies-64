//
// Created by Shadow-Link on 04/03/2023.
//

#ifndef N64_ACHIEVEMENT_COINS_TROPHY_H
#define N64_ACHIEVEMENT_COINS_TROPHY_H

#include <stdbool.h>
#include <stdio.h>

typedef enum {
    BRONZE,
    SILVER,
    GOLD,
    PLATINUM
} TrophyLevel;

typedef enum {
    COUNTER,
    BOOL
} TrophyType;

typedef struct {
    TrophyLevel level;
    TrophyType type;
    char title[50];
    char description[120];
    int targetCount;
    int currentCount;
    bool isCollected;
    bool containsSpoilers;
} Trophy;

#endif //N64_ACHIEVEMENT_COINS_TROPHY_H
