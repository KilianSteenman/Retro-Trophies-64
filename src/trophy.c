//
// Created by Shadow-Link on 04/03/2023.
//

int isTrophyCollected(FILE *saveState, Trophy trophy) {
    int result = 1;
    if (trophy.type == OR) {
        result = 0;
    } else {
        result = 1;
    }

    // Check if all requirements are fulfilled
    for (int i = 0; i < trophy.requirementCount; i++) {
        Requirement requirement = trophy.requirements[i];

        fseek(saveState, requirement.address, SEEK_SET);

        char buffer[1];
        fread(buffer, 1, sizeof(buffer), saveState);
        int bit = (buffer[0] & requirement.value) == requirement.value;
        if (trophy.type == OR) {
            result = result | bit;
        } else if (trophy.type == COUNT) {
            result = result & bit;
        }
    }

    return result;
}