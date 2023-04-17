#include <stdio.h>
#include <libdragon.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "trophy.h"
#include "debug.h"
#include "graphics_ext.h"
#include "games/1080_snowboarding.h"
#include "games/super_mario_64.h"
#include "games/super_smash_bros.h"
#include "games/mario_kart_64.h"
#include "games/zelda_oot.h"

typedef enum {
    GAME_SELECT,
    TROPHY_OVERVIEW
} State;

State state = GAME_SELECT;
Game *selectedGame;
int selectedGameIndex = 0;
int selectedTrophy = 0;

void draw_trophy(int x, int y, display_context_t disp, Trophy trophy) {
    graphics_draw_text(disp, x, y, trophy.title);
    graphics_draw_text(disp, x, y + 10, trophy.description);

    if (trophy.type == COUNTER) {
        char buffer[100];
        sprintf(buffer, "%d / %d", trophy.currentCount, trophy.targetCount);

        int percentageCompleted = ((float) trophy.currentCount / trophy.targetCount * 100.0f);
        graphics_draw_text(disp, x + 400, y, buffer);
        graphics_draw_progressbar(disp, x + 400, y + 10, 100, 10, graphics_make_color(255, 255, 255, 255),
                                  graphics_make_color(255, 0, 0, 255), percentageCompleted);
    }
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

void draw_game_tile(display_context_t disp, int x, int y, Game game) {
    graphics_draw_box(disp, x, y, 620, 25, graphics_make_color(14, 128, 17, 255));
    graphics_draw_text(disp, x + 5, y + 5, game.title);

    int bronzeCount = 0;
    int silverCount = 0;
    int goldCount = 0;
    int percentageCompleted = 0;
    getGameStatus(game, &bronzeCount, &silverCount, &goldCount, &percentageCompleted);

    // Percentage
    char percentageBuffer[4];
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

    // TODO: Clean this mess up, there are much easier ways to deal with this
    int startIndex = 0;
    int endIndex = game.trophyCount;
    if (endIndex > 7) {
        endIndex = 7;
    }
    if (selectedTrophy >= 7) {
        startIndex = selectedTrophy - 6;
        endIndex = startIndex + 7;
        if (endIndex > game.trophyCount) {
            endIndex = game.trophyCount;
        }
    }

    for (int i = startIndex; i < endIndex; i++) {
        if (i == selectedTrophy) {
            graphics_set_color(0xFFFFFFFF, 0x0);
        } else if (game.trophies[i].isCollected == 1) {
            graphics_set_color(graphics_make_color(0, 255, 0, 255), 0x0);
        } else {
            graphics_set_color(graphics_make_color(0, 0, 255, 255), 0x0);
        }
        draw_trophy(x, ((30 * (i - startIndex)) + y), disp, game.trophies[i]);
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

void loadGameData(Game *game, char *saveGame, void (*f)(Game *, FILE *)) {
    FILE *saveState = fopen(saveGame, "r");
    if (saveState == NULL) {
        printf("Save data unavailable for %s\n", game->title);
        return;
    }

    (*f)(game, saveState);

    fclose(saveState);
}

typedef struct {
    char game_path[257];
    char file_name[100];
} GamePath;

typedef enum {
    RAM,
    EEP
} SaveType;

typedef void (*TrophyDataLoader)(Game *, FILE *);

typedef struct {
    char name[50];
    char game_code[4];
    SaveType save_type;
    TrophyDataLoader trophy_data_loader;
} SupportedGame;

typedef struct {
    SupportedGame supported_game;
    char filename[100];
} DetectedGame;

char *strip_extension(const char *filename) {
    size_t len = strlen(filename);
    char *newfilename = malloc(len - 3);
    if (!newfilename) /* handle error */;
    memcpy(newfilename, filename, len - 4);
    newfilename[len - 4] = 0;
    return newfilename;
}

void print_dir(char *dir, SupportedGame *supported_games, int supported_game_count, DetectedGame *detected_games,
               int *detected_game_count) {
    printf("Supported games:\n");
    for (int i = 0; i < supported_game_count; i++) {
        printf("%s - %s\n", supported_games[i].game_code, supported_games[i].name);
    }

    printf("Detecting games:\n");
    int path_count = 0;
    GamePath paths[10];
//    int counter = 0;

    dir_t buf;
    int ret = dir_findfirst(dir, &buf);
    while (ret == 0 && buf.d_name[0] != '\0') {
        if (buf.d_type == DT_DIR) {
            //printf("[%d] Dir %s %d\n", counter++, buf.d_name, buf.d_type);
//            char next_dir[512];
            //sprintf(next_dir, "%s%s/", dir, buf.d_name);
            //printf("next: %s\n", next_dir);
//            print_dir(next_dir);
        } else {
//            printf("[%d] File '%s' %d\n", counter++, buf.d_name, buf.d_type);

            // Check if this is an N64 rom
            char *dot = strrchr(buf.d_name, '.');
            if (dot && !strcmp(dot, ".z64") && buf.d_name[0] != '.') {
                printf("Z64 file detected %s\n", buf.d_name);
                sprintf(paths[path_count].game_path, "%s%s", dir, buf.d_name);
                strcpy(paths[path_count].file_name, strip_extension(buf.d_name));
                path_count++;
            }
        }
        ret = dir_findnext(dir, &buf);
    }

    printf("Found %d games... Detecting game type\n", path_count);
    char game_id[3];
    for (int i = 0; i < path_count; i++) {
        printf("Detecting game %s\n", paths[i].game_path);
        // Checks the 'unique' identifier for this rom
        FILE *game_rom = fopen(paths[i].game_path, "r");
//        printf("Opened game %s\n", paths[i].game_path);
        if (game_rom == NULL) {
            debug_print_and_stop("Unable to open game rom");
            return;
        }
        // Game ID is at 60 (well 59 actually)
        fseek(game_rom, 60, SEEK_SET);
        fread(game_id, 3, 1, game_rom);
        fclose(game_rom);

        // Checks if the game is a supported game
        char is_supported = 0;
        for (int sg = 0; sg < supported_game_count; sg++) {
            if (strncmp(game_id, supported_games[sg].game_code, 3) == 0) {
                printf("Detected %s\n", supported_games[sg].name);
                is_supported = 1;

                strcpy(detected_games[*detected_game_count].filename, paths[i].file_name);
                detected_games[*detected_game_count].supported_game = supported_games[sg];
                *detected_game_count = *detected_game_count + 1;
            }
        }

        if (is_supported == 0) {
            printf("Unknown game detected '%.3s' at %s \n", game_id, paths[i].game_path);
        }
    }
    printf("Finished game detection\n");
}

void detect_games(SupportedGame *supported_games, int supported_game_count, DetectedGame *detected_games,
                  int *detected_game_count) {
#ifdef N64_HARDWARE
    if (!debug_init_sdfs("sd:/", -1)) {
#else
    if (dfs_init(DFS_DEFAULT_LOCATION) != DFS_ESUCCESS) {
#endif
        debug_print_and_stop("Error opening SD");
    }

#ifdef N64_HARDWARE
    print_dir("sd:/", supported_games, supported_game_count, detected_games, detected_game_count);
#else
    print_dir("rom://", supported_games, supported_game_count, detected_games, detected_game_count);
#endif
}

char *get_extension_for_save_type(SaveType saveType) {
    switch (saveType) {
        case RAM:
            return "srm";
        case EEP:
            return "eep";
        default:
            return "ram";
    }
}

int main(void) {
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);

    console_init();
    controller_init();

    debug_init_usblog();
    console_set_debug(true);

    // TODO: Make this dynamic
    SupportedGame supported_games[5];
    supported_games[0] = (SupportedGame) {.name = "1080 Snowboarding", .game_code = "TEA", .save_type = RAM, .trophy_data_loader = get_game_data_1080};
    supported_games[1] = (SupportedGame) {.name = "Super Smash Bros", .game_code = "ALE", .save_type = RAM, .trophy_data_loader = get_game_data_super_smash_bros};
    supported_games[2] = (SupportedGame) {.name = "Super Mario 64", .game_code = "SME", .save_type = EEP, .trophy_data_loader = get_game_data_mario64};
    supported_games[3] = (SupportedGame) {.name = "Mario Kart 64", .game_code = "KTE", .save_type = EEP, .trophy_data_loader = get_game_data_mario_kart_64};
    supported_games[4] = (SupportedGame) {.name = "The Legend of Zelda: Ocarina of Time", .game_code = "ZLP", .save_type = RAM, .trophy_data_loader = get_trophy_data_zelda_oot};

    DetectedGame detected_games[50];
    int detected_game_count = 0;
    detect_games(supported_games, 5, detected_games, &detected_game_count);

    debug_print_and_pause("Loading trophy data\n");
    // TODO: Make this dynamic
    Game games[6];
    for (int i = 0; i < detected_game_count; i++) {
        printf("Loading trophy data for %s\n", detected_games[i].filename);
        char save_path[512];
#ifdef N64_HARDWARE
        sprintf(save_path, "sd:/ED64/gamedata/%s.%s", detected_games[i].filename,
                get_extension_for_save_type(detected_games[i].supported_game.save_type));
#else
        sprintf(save_path, "rom://ED64/gamedata/%s.%s", detected_games[i].filename,
                get_extension_for_save_type(detected_games[i].supported_game.save_type));
#endif
        printf("Loading game data '%s'\n", save_path);

        strcpy(games[i].title, detected_games[i].supported_game.name);
        games[i].trophyCount = 0;
        loadGameData(&games[i], save_path, detected_games[i].supported_game.trophy_data_loader);
    }

    debug_print_and_pause("Loaded game data\n");

    while (1) {
        /* Grab a render buffer */
        display_context_t disp = display_lock();

        /* Fill the screen */
        graphics_fill_screen(disp, 0x0);

        /* Render the screen */
        if (state == GAME_SELECT) {
            render_game_select_screen(disp, games, detected_game_count);
        } else if (state == TROPHY_OVERVIEW) {
            render_screen(disp, *selectedGame);
        }

        /* Force backbuffer flip */
        display_show(disp);
    }
}
