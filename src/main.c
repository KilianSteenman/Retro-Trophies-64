#include <stdio.h>
#include <libdragon.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "trophy.h"

typedef enum {
    GAME_SELECT,
    TROPHY_OVERVIEW
} State;

sprite_t *testSprite;

State state = GAME_SELECT;
Game *selectedGame;
int selectedGameIndex = 0;
int selectedTrophy = 0;

void draw_trophy(int x, int y, display_context_t disp, Trophy trophy) {
    graphics_draw_text(disp, x, y, trophy.title);
    graphics_draw_text(disp, x, y + 10, trophy.description);
}

void
get_trophy_totals(Game *games, int gameCount, int *bronzeCount, int *silverCount, int *goldCount, int *completedCount) {
    for (int g = 0; g < gameCount; g++) {
        Game game = games[g];
        for (int i = 0; i < game.trophyCount; i++) {
            if (!game.trophies[i].isCollected) {
                continue;
            }

            switch (game.trophies[i].level) {
                case BONUS:
                    *bronzeCount = *bronzeCount + 1;
                    break;
                case MILESTONE:
                    *silverCount = *silverCount + 1;
                    break;
                case FINISHED:
                    *goldCount = *goldCount + 1;
                    break;
                case COMPLETED:
                    *completedCount = *completedCount + 1;
                    break;
            }
        }
    }
}

void graphics_draw_number(display_context_t disp, int x, int y, int number) {
    char buffer[11];
    sprintf(buffer, "%d", number);
    graphics_draw_text(disp, x, y, buffer);
}

void graphics_draw_progressbar(display_context_t disp, int x, int y, int width, int height, uint32_t bgColor,
                               uint32_t fillColor, int percentageCompleted) {
    graphics_draw_box(disp, x, y, width, height, bgColor);
    graphics_draw_box(disp, x, y, width * ((float) percentageCompleted / 100.0f), height, fillColor);
}

void draw_game_tile(display_context_t disp, int x, int y, Game game) {
    graphics_draw_box(disp, x, y, 620, 25, graphics_make_color(14, 128, 17, 255));
    graphics_draw_text(disp, x + 5, y + 5, game.title);

    int bronzeCount = 0;
    int silverCount = 0;
    int goldCount = 0;
    int percentageCompleted = 0;
    getGameStatus(game, &bronzeCount, &silverCount, &goldCount, &percentageCompleted);

    // Percentage
    char percentageBuffer[12];
    sprintf(percentageBuffer, "%d%%", percentageCompleted);
    graphics_draw_text(disp, x + 450, y + 5, percentageBuffer);
    graphics_draw_progressbar(disp, x + 450, y + 15, 30, 3, graphics_make_color(255, 255, 255, 255),
                              graphics_make_color(255, 0, 0, 255), percentageCompleted);

    // Trophy counts
    graphics_draw_number(disp, x + 500, y + 5, bronzeCount);
    graphics_draw_number(disp, x + 530, y + 5, silverCount);
    graphics_draw_number(disp, x + 560, y + 5, goldCount);
}

void on_game_selected(Game *game) {
    selectedGame = game;
    state = TROPHY_OVERVIEW;
    selectedTrophy = 0;
}

void render_game_select_screen(display_context_t disp, Game *games, int gameCount) {
    // draw totals
    int bronzeCount = 0, silverCount = 0, goldCount = 0, completedCount = 0;
    get_trophy_totals(games, gameCount, &bronzeCount, &silverCount, &goldCount, &completedCount);
    graphics_set_color(0xFF0000FF, 0x0);
    graphics_draw_number(disp, 10, 10, bronzeCount);
    graphics_draw_number(disp, 50, 10, silverCount);
    graphics_draw_number(disp, 90, 10, goldCount);
    graphics_draw_number(disp, 130, 10, completedCount);

    for (int i = 0; i < gameCount; i++) {

        /* Set the text output color */
        if (i == selectedGameIndex) {
            graphics_set_color(0xFFFFFFFF, 0x0);
        } else {
            graphics_set_color(0xFF0000FF, 0x0);
        }

        draw_game_tile(disp, 10, i * 30 + 30, games[i]);
    }

    // Check controller input
    controller_scan();
    struct controller_data ckeys = get_keys_down();

    if (ckeys.c[0].down) {
        selectedGameIndex++;
    } else if (ckeys.c[0].up) {
        selectedGameIndex--;
    }

    if (selectedGameIndex == -1) {
        selectedGameIndex = gameCount - 1;
    } else if (selectedGameIndex == gameCount) {
        selectedGameIndex = 0;
    }

    if (ckeys.c[0].A) {
        on_game_selected(&games[selectedGameIndex]);
    }
}

void render_screen(display_context_t disp, Game game) {
    /* Set the text output color */
    graphics_set_color(0xFFFFFFFF, 0x0);

    int x = 10;
    int y = 10;
    for (int i = 0; i < game.trophyCount; i++) {
        if (i == selectedTrophy) {
            graphics_set_color(0xFFFFFFFF, 0x0);
        } else if (game.trophies[i].isCollected == 1) {
            graphics_set_color(graphics_make_color(0, 255, 0, 255), 0x0);
        } else {
            graphics_set_color(graphics_make_color(0, 0, 255, 255), 0x0);
        }
        draw_trophy(x, ((30 * i) + y), disp, game.trophies[i]);
    }

    // Check controller input
    controller_scan();
    struct controller_data ckeys = get_keys_down();

    if (ckeys.c[0].down) {
        selectedTrophy++;
    } else if (ckeys.c[0].up) {
        selectedTrophy--;
    }

    if (selectedTrophy == -1) {
        selectedTrophy = game.trophyCount - 1;
    } else if (selectedTrophy == game.trophyCount) {
        selectedTrophy = 0;
    }

    if (ckeys.c[0].B) {
        state = GAME_SELECT;
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
            } else if(strcmp(type, "BOOL") == 0) {
                trophies[i].type = BOOL;
            }

            strcpy(trophies[i].title, title);
            strcpy(trophies[i].description, description);
            trophies[i].requirements = (Requirement *) malloc(sizeof(Requirement) * count);
            trophies[i].requirementCount = count;

            // Load requirement data
            for (int c = 0; c < count; c++) {
                if (fgets(line, 150, trophyFile) != NULL) {
                    ptr = strtok(line, delim);
                    strcpy(level, ptr);

                    ptr = strtok(NULL, delim);
                    strcpy(title, ptr);
                    ptr = strtok(NULL, delim);
                    strcpy(description, ptr);
                    ptr = strtok(NULL, delim);
                    strcpy(type, ptr);
                    ptr = strtok(NULL, delim);
                    count = (int) strtol(ptr, NULL, 10);

                    char address[10];
                    char value[10];

                    sscanf(line, "%s %s", address, value);
                    int addressValue = (int) strtol(address, NULL, 16);
                    int valueValue = (int) strtol(value, NULL, 2);

                    Requirement requirement = {.address = addressValue, .value = valueValue};
                    trophies[i].requirements[c] = requirement;
                }
            }
        }
    }

    return trophies;
}

int isTrophyCollected(FILE *saveState, Trophy trophy) {
    int result = 1;
    if (trophy.type == OR) {
        result = 0;
    } else {
        result = 1;
    }

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

int loadGameData(Game *game, char *title, char *saveGame, char *trophyFile) {
    FILE *coinData = fopen(trophyFile, "r");
    if (!coinData) {
        printf("Error loading coin data\n");
        return 0;
    }

    strcpy(game->title, title);
    game->trophies = loadTrophyData(coinData, &game->trophyCount);
    fclose(coinData);

    // Check if trophies are collected
    FILE *saveState = fopen(saveGame, "r");
    if (saveState == NULL) {
        printf("Save state not available");
        return 0;
    }
    for (int i = 0; i < game->trophyCount; i++) {
        game->trophies[i].isCollected = isTrophyCollected(saveState, game->trophies[i]);
    }
    fclose(saveState);
    return 1;
}

int main(void) {
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);

    console_init();
    controller_init();

    debug_init_usblog();
    console_set_debug(true);

    // Test sprite rendering
    int fp = dfs_open("/earthbound.sprite");
    testSprite = malloc(dfs_size(fp));
    dfs_read(testSprite, 1, dfs_size(fp), fp);
    dfs_close(fp);

    int gameCount = 1;
    Game games[gameCount];
    loadGameData(&games[0], "1080 Snowboarding", "rom:/1080.ram", "rom:/1080.dat");
//    loadGameData(&games[1], "Super Mario 64", "rom:/SuperMario64.eep", "rom:/MARIO64.dat");
//    loadGameData(&games[2], "Super Mario 64 - 100%", "rom:/SuperMario64_100.eep", "rom:/MARIO64.dat");
//    loadGameData(&games[3], "Super Smash bros", "rom:/ssb_cf_unlocked.ram", "rom:/SSB.dat");

    while (1) {
        /* Grab a render buffer */
        display_context_t disp = display_lock();

        /* Fill the screen */
        graphics_fill_screen(disp, 0x0);

        /* Render the screen */
        if (state == GAME_SELECT) {
            render_game_select_screen(disp, games, gameCount);
        } else if (state == TROPHY_OVERVIEW) {
            render_screen(disp, *selectedGame);
        }

        //rdp_detach_display();

        /* Force backbuffer flip */
        display_show(disp);
    }
}
