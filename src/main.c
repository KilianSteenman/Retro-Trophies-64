#include <stdio.h>
#include <libdragon.h>
#include <stdlib.h>
#include <string.h>

#include "trophy.h"

void draw_trophy(int x, int y, display_context_t disp, Trophy trophy) {
    graphics_draw_text(disp, x, y, trophy.title);
    graphics_draw_text(disp, x, y + 10, trophy.description);
}

static int selected = 0;

void render_screen(display_context_t disp, Trophy *trophies, int trophyCount) {
    /* Set the text output color */
    graphics_set_color(0xFFFFFFFF, 0x0);

    int x = 10;
    int y = 10;
    for (int i = 0; i < trophyCount; i++) {
        if (i == selected) {
            graphics_set_color(0xFFFFFFFF, 0x0);
        } else if (trophies[i].isCollected == 1) {
            graphics_set_color(graphics_make_color(0, 255, 0, 255), 0x0);
        } else {
            graphics_set_color(graphics_make_color(0, 0, 255, 255), 0x0);
        }
        draw_trophy(x, ((30 * i) + y), disp, trophies[i]);
    }
}

Trophy *loadTrophyData(FILE *trophyFile, int *trophyCount) {
    char line[150];

    // Read trophy count
    if (fgets(line, 150, trophyFile) != NULL) {
        sscanf(line, "TROPHY_COUNT %d", trophyCount);
    }
    printf("Trophy Count: %d\n", *trophyCount);

    char level[10];
    char title[50];
    char description[120];
    char type[10];
    int count;

    // Load trophy data
    Trophy *trophies = (Trophy *) malloc(sizeof(Trophy) * *trophyCount);
    for (int i = 0; i < *trophyCount; i++) {
        // Read a trophy
        if (fgets(line, 150, trophyFile) != NULL) {

            // Sample: STAR;Stargazer;COUNT;120
            char delim[] = ";";
            char *ptr = strtok(line, delim);

            strcpy(level, ptr);
            ptr = strtok(NULL, delim);
            strcpy(title, ptr);
            ptr = strtok(NULL, delim);
            strcpy(description, ptr);
            ptr = strtok(NULL, delim);
            strcpy(type, ptr);
            ptr = strtok(NULL, delim);
            count = (int) strtol(ptr, NULL, 10);

            // Parse type info
            if (strcmp(type, "COUNT") == 0) {
                trophies[i].type = COUNT;
            } else if (strcmp(type, "OR") == 0) {
                trophies[i].type = OR;
            }

            strcpy(trophies[i].title, title);
            strcpy(trophies[i].description, description);
            trophies[i].requirements = (Requirement *) malloc(sizeof(Requirement) * count);
            trophies[i].requirementCount = count;
            for (int c = 0; c < count; c++) {
                if (fgets(line, 150, trophyFile) != NULL) {
                    char address[10];
                    char value[10];

                    sscanf(line, "%s %s", address, value);
                    int addressValue = (int) strtol(address, NULL, 16);
                    int valueValue = (int) strtol(value, NULL, 2);
//                    printf("Address %d value %d (%s)\n", addressValue, valueValue, value);

                    Requirement requirement = {.address = addressValue, .value = valueValue};
                    trophies[i].requirements[c] = requirement;
                }
            }
        }
    }

    return trophies;
}

int main(void) {
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);

    console_init();
    controller_init();

    debug_init_usblog();
    console_set_debug(true);

    FILE *fp = fopen("rom:/MARIO64.dat", "r");
    if (!fp) {
        printf("Error loading coin data\n");
        return 0;
    }

    int trophyCount;
    Trophy *trophies = loadTrophyData(fp, &trophyCount);
    fclose(fp);

    while (1) {
        /* Grab a render buffer */
        display_context_t disp = display_lock();

        /* Fill the screen */
        graphics_fill_screen(disp, 0x0);

        /* Render the screen */
        render_screen(disp, trophies, trophyCount);

        /* Force backbuffer flip */
        display_show(disp);

        // Check controller input
        controller_scan();
        struct controller_data ckeys = get_keys_down();

        if (ckeys.c[0].start) {
            if (selected == 7) {
                selected = 0;
            } else {
                selected++;
            }
        }
    }
}
