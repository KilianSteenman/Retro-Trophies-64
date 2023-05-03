//
// Created by Shadow-Link on 03/05/2023.
//

#include "controller_ext.h"

bool is_down_pressed(struct controller_data down) {
    if(down.c[0].down) {
        return true;
    }

    if(down.c[0].y < -80) {
        return true;
    }

    return false;
}

bool is_up_pressed(struct controller_data down) {
    if(down.c[0].up) {
        return true;
    }

    if(down.c[0].y > 80) {
        return true;
    }

    return false;
}
