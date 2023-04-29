//
// Created by Shadow-Link on 29/04/2023.
//

#include <stdlib.h>

#include "list_selection.h"

ListSelection *list_selection_new(int itemCount, int maxIndex) {
    ListSelection *ls = malloc(sizeof(ListSelection));
    ls->itemCount = itemCount;
    ls->maxIndex = maxIndex;

    ls->selectedIndex = 0;
    ls->startIndex = 0;
    ls->endIndex = itemCount;
    return ls;
}

void move_up(ListSelection *ls) {
    ls->selectedIndex--;
    if (ls->selectedIndex < 0) {
        ls->selectedIndex = ls->maxIndex - 1;
        ls->endIndex = ls->maxIndex;
        ls->startIndex = ls->maxIndex - ls->itemCount;

        // Make sure startIndex is within bounds
        if (ls->startIndex < 0) {
            ls->startIndex = 0;
        }
    }

    if (ls->selectedIndex < ls->startIndex) {
        ls->startIndex = ls->selectedIndex;
        ls->endIndex = ls->startIndex + ls->itemCount;
    }

    // Don't exceed the bottom of the list
    if (ls->endIndex > ls->maxIndex) {
        ls->endIndex = ls->maxIndex;
    }
}

void move_down(ListSelection *ls) {
    ls->selectedIndex++;
    if (ls->selectedIndex >= ls->maxIndex) {
        ls->selectedIndex = 0;
        ls->startIndex = 0;
        ls->endIndex = ls->itemCount;
    }

    // Start moving down the list when we reached the bottom of the screen
    if ((ls->selectedIndex - ls->startIndex) >= ls->itemCount) {
        ls->startIndex = ls->selectedIndex - ls->itemCount + 1;
        ls->endIndex = ls->startIndex + ls->itemCount;
    }

    // Don't exceed the bottom of the list
    if (ls->endIndex > ls->maxIndex) {
        ls->endIndex = ls->maxIndex;
    }
}
