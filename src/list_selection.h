//
// Created by Shadow-Link on 29/04/2023.
//

#ifndef N64_TROPHIES_LISTSELECTION_H
#define N64_TROPHIES_LISTSELECTION_H

typedef struct {
    int startIndex;
    int selectedIndex;
    int endIndex;

    int itemCount;
    int maxIndex;
} ListSelection;

ListSelection *list_selection_new(int itemCount, int maxIndex);
void move_up(ListSelection *ls);
void move_down(ListSelection *ls);

#endif //N64_TROPHIES_LISTSELECTION_H
