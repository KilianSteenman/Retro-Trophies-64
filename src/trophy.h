//
// Created by Shadow-Link on 04/03/2023.
//

#ifndef N64_ACHIEVEMENT_COINS_TROPHY_H
#define N64_ACHIEVEMENT_COINS_TROPHY_H

typedef struct Requirement {
    int address;
    int value;
} Requirement;

typedef enum TrophyLevel {
    BONUS,
    MILESTONE,
    FINISHED,
    COMPLETED
} TrophyLevel;

typedef enum RequirementType {
    COUNT,
    OR,
    BOOL
} RequirementType;

typedef struct Trophy {
    TrophyLevel level;
    RequirementType type;
    char title[50];
    char description[120];
    int requirementCount;
    Requirement *requirements;
    int isCollected;
} Trophy;

int isTrophyCollected(FILE *saveState, Trophy trophy);

#endif //N64_ACHIEVEMENT_COINS_TROPHY_H
