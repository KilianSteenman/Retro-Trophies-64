#include <stdio.h>
#include <libdragon.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "trophy.h"
#include "./games/1080.c"
#include "./games/mario64.c"
#include "./games/super_smash_bros.c"

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
                case BRONZE:
                    *bronzeCount = *bronzeCount + 1;
                    break;
                case SILVER:
                    *silverCount = *silverCount + 1;
                    break;
                case GOLD:
                    *goldCount = *goldCount + 1;
                    break;
                case PLATINUM:
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

void loadGameData(Game *game, char *saveGame, void (*f)(Game*, FILE*)) {
    FILE *saveState = fopen(saveGame, "r");
    if (saveState == NULL) {
        printf("Save state not available");
        return;
    }

    (*f)(game, saveState);

    fclose(saveState);
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

    int gameCount = 4;
    Game games[gameCount];
    loadGameData(&games[0], "rom:/1080.ram", get_game_data_1080);
    loadGameData(&games[1], "rom:/SuperMario64.eep", get_game_data_mario64);
    loadGameData(&games[2], "rom:/SuperMario64_100.eep", get_game_data_mario64);
    loadGameData(&games[3], "rom:/ssb_cf_unlocked.ram", get_game_data_super_smash_bros);
//    loadGameData(&games[0], "1080 Snowboarding", "rom:/1080.ram", "rom:/1080.dat");
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

        /* Force backbuffer flip */
        display_show(disp);
    }
}
