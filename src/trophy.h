//
// Created by Shadow-Link on 04/03/2023.
//

#ifndef N64_ACHIEVEMENT_COINS_TROPHY_H
#define N64_ACHIEVEMENT_COINS_TROPHY_H

#include <stdio.h>

typedef struct {
    int address;
    int value;
} Requirement;

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
    char isCollected;
} Trophy;

#endif //N64_ACHIEVEMENT_COINS_TROPHY_H
