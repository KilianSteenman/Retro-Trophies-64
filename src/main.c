#include <stdio.h>
#include <libdragon.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "controller_ext.h"
#include "game.h"
#include "trophy.h"
#include "debug.h"
#include "graphics_ext.h"
#include "list_selection.h"
#include "ui.h"
#include "save_type.h"
#include "supported_games.h"

// Trick to pass in the VERSION via cflags
#define xstr(s) str(s)
#define str(s) #s

#ifndef VERSION
#define VERSION dev
#endif

char *version = xstr(VERSION);

typedef enum {
    ABOUT,
    GAME_SELECT,
    TROPHY_OVERVIEW
} State;

State state = GAME_SELECT;
Game *selectedGame;
bool show_game_info;

sprite_t *locked;
sprite_t *bronze;
sprite_t *silver;
sprite_t *gold;
sprite_t *splash;

sprite_t *button_a;
sprite_t *button_b;

ListSelection *gameSelection;
ListSelection *trophySelection;

const char *spoilerDescription = "Trophy contains spoilers";

char about[20];

sprite_t *get_trophy_sprite(Trophy *trophy) {
    if (trophy->isCollected) {
        switch (trophy->level) {
            case BRONZE:
                return bronze;
            case SILVER:
                return silver;
            case GOLD:
                return gold;
            default:
                return locked;
        }
    } else {
        return locked;
    }
}

void render_footer(display_context_t disp) {
    graphics_set_color(SCREEN_TITLE_COLOR, 0x0);
    graphics_draw_text(disp, 230, 225, about);
}

void draw_trophy(int x, int y, display_context_t disp, Trophy trophy, bool is_selected, bool show_spoiler) {
    char description[120];
    if (trophy.containsSpoilers && !(is_selected && show_spoiler)) {
        strcpy(description, spoilerDescription);
    } else {
        strcpy(description, trophy.description);
    }

    uint32_t tile_background_color = TILE_DEFAULT_BACKGROUND_COLOR;
    uint32_t tile_border_color = TILE_DEFAULT_BORDER_COLOR;
    uint32_t tile_text_color = TILE_DEFAULT_TEXT_COLOR;
    if (is_selected) {
        tile_background_color = TILE_SELECTED_BACKGROUND_COLOR;
        tile_border_color = TILE_SELECTED_BORDER_COLOR;
        tile_text_color = TILE_SELECTED_TEXT_COLOR;
    }

    graphics_draw_bordered_box(disp, x, y, 620, 30, tile_background_color, tile_border_color, BORDER_THICKNESS);
    graphics_draw_sprite_trans(disp, x + 16, y + 6, get_trophy_sprite(&trophy));
    graphics_set_color(tile_text_color, 0x0);
    graphics_draw_text(disp, x + 48, y + 6, trophy.title);
    graphics_draw_text(disp, x + 48, y + 16, description);

    if (trophy.type == COUNTER) {
        char buffer[100];
        sprintf(buffer, "%d / %d", trophy.currentCount, trophy.targetCount);

        int percentageCompleted = ((float) trophy.currentCount / trophy.targetCount * 100.0f);
        graphics_draw_text(disp, x + 500, y + 6, buffer);
        graphics_draw_progressbar(disp, x + 500, y + 16, 70, 5, PROGRESSBAR_BACKGROUND_COLOR,
                                  PROGRESSBAR_FOREGROUND_COLOR, percentageCompleted);
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

void draw_trophy_counter(display_context_t disp, int x, int y, int count, sprite_t *sprite) {
    graphics_draw_number(disp, x + 20, y + 5, count);
    graphics_draw_sprite_trans(disp, x, y, sprite);
}

void draw_game_tile(display_context_t disp, int x, int y, Game game, bool is_selected) {
    uint32_t tile_background_color = TILE_DEFAULT_BACKGROUND_COLOR;
    uint32_t tile_border_color = TILE_DEFAULT_BORDER_COLOR;
    uint32_t tile_text_color = TILE_DEFAULT_TEXT_COLOR;
    if (is_selected) {
        tile_background_color = TILE_SELECTED_BACKGROUND_COLOR;
        tile_border_color = TILE_SELECTED_BORDER_COLOR;
        tile_text_color = TILE_SELECTED_TEXT_COLOR;
    }
    graphics_draw_bordered_box(disp, x, y, 620, 30, tile_background_color, tile_border_color, BORDER_THICKNESS);
    graphics_set_color(tile_text_color, 0x0);
    graphics_draw_text(disp, x + 15, y + 10, game.title);

    int bronzeCount = 0;
    int silverCount = 0;
    int goldCount = 0;
    int percentageCompleted = 0;
    getGameStatus(game, &bronzeCount, &silverCount, &goldCount, &percentageCompleted);

    // Percentage
    char percentageBuffer[4];
    sprintf(percentageBuffer, "%d%%", percentageCompleted);
    graphics_draw_text(disp, x + 425, y + 8, percentageBuffer);
    graphics_draw_progressbar(disp, x + 425, y + 18, 30, 3, PROGRESSBAR_BACKGROUND_COLOR, PROGRESSBAR_FOREGROUND_COLOR,
                              percentageCompleted);

    // Trophy counts
    draw_trophy_counter(disp, OFFSET_TROPHY_BRONZE_X, y + 6, bronzeCount, bronze);
    draw_trophy_counter(disp, OFFSET_TROPHY_SILVER_X, y + 6, silverCount, silver);
    draw_trophy_counter(disp, OFFSET_TROPHY_GOLD_X, y + 6, goldCount, gold);
}

void on_game_selected(Game *game) {
    selectedGame = game;
    state = TROPHY_OVERVIEW;

    trophySelection->selectedIndex = 0;
    trophySelection->startIndex = 0;
    trophySelection->endIndex = trophySelection->itemCount;
    trophySelection->maxIndex = game->trophyCount;
    if (trophySelection->maxIndex <= trophySelection->itemCount) {
        trophySelection->endIndex = trophySelection->maxIndex;
    }
}

void render_about_screen(display_context_t disp) {
    // Update

    // Check controller input
    controller_scan();
    struct controller_data keys_down = get_keys_down();
    if (keys_down.c[0].B) {
        state = GAME_SELECT;
    }

    // Render
    graphics_draw_sprite_trans(disp, 300, 100, splash);

    graphics_set_color(SCREEN_TITLE_COLOR, 0x0);
    graphics_draw_text(disp, 240, 140, "Retro Trophies 64");
    graphics_draw_text(disp, 300, 150, "by");
    graphics_draw_text(disp, 265, 160, "Shadow-Link");
}

void render_button(display_context_t disp, int x, int y, char *text, sprite_t *sprite) {
//    graphics_draw_sprite_trans(disp, x, y, sprite);
}

void render_game_info(display_context_t disp, Game *game) {
    char buffer[200];

    graphics_draw_bordered_box(disp, 120, 70, 400, 100, TILE_DEFAULT_BACKGROUND_COLOR, TILE_DEFAULT_BORDER_COLOR,
                               BORDER_THICKNESS);
    graphics_set_color(SCREEN_TITLE_COLOR, 0x0);
    graphics_draw_text(disp, 140, 80, "Game info");
    graphics_set_color(TILE_DEFAULT_TEXT_COLOR, 0x0);
    sprintf(buffer, "Title:  %s", game->title);
    graphics_draw_text(disp, 140, 100, buffer);
    sprintf(buffer, "File:   %s", game->filename);
    graphics_draw_text(disp, 140, 110, buffer);
    if (game->region == PAL) {
        sprintf(buffer, "Region: PAL");
    } else {
        sprintf(buffer, "Region: USA");
    }
    graphics_draw_text(disp, 140, 120, buffer);
}

void render_game_select_screen(display_context_t disp, Game *games, int gameCount) {
    // Check controller input
    controller_scan();
    struct controller_data keys_down = get_keys_down();

    if (is_down_pressed(keys_down)) {
        move_down(gameSelection);
    } else if (is_up_pressed(keys_down)) {
        move_up(gameSelection);
    }

    if (gameCount > 0 && keys_down.c[0].A) {
        on_game_selected(&games[gameSelection->selectedIndex]);
    } else if (keys_down.c[0].R) {
        state = ABOUT;
    } else if(keys_down.c[0].L) {
        show_game_info = !show_game_info;
    }

    // Render

    // Header
    graphics_set_color(SCREEN_TITLE_COLOR, 0x0);
    graphics_draw_text(disp, 20, 15, "Retro Trophies 64");

    // Totals
    int bronzeCount = 0, silverCount = 0, goldCount = 0, completedCount = 0;
    get_trophy_totals(games, gameCount, &bronzeCount, &silverCount, &goldCount, &completedCount);
    graphics_set_color(TOTALS_COLOR, 0x0);
    draw_trophy_counter(disp, OFFSET_TROPHY_BRONZE_X, 10, bronzeCount, bronze);
    draw_trophy_counter(disp, OFFSET_TROPHY_SILVER_X, 10, silverCount, silver);
    draw_trophy_counter(disp, OFFSET_TROPHY_GOLD_X, 10, goldCount, gold);

    if (gameCount > 0) {
        // Game list
        for (int i = gameSelection->startIndex; i < gameSelection->endIndex; i++) {
            bool is_selected = i == gameSelection->selectedIndex;
            draw_game_tile(disp, 10, (i - gameSelection->startIndex) * 31 + HEADER_HEIGHT, games[i], is_selected);
        }
    } else {
        graphics_draw_text(disp, 200, 120, "No supported games detected");
    }

    // Render game information
    if (gameCount > 0 && show_game_info) {
        render_game_info(disp, &games[gameSelection->selectedIndex]);
    }

    // Footer
    render_button(disp, 100, 100, "View trophies", button_a);
}

void render_trophy_screen(display_context_t disp, Game game) {
    // Update

    // Check controller input
    controller_scan();
    struct controller_data keys_down = get_keys_down();

    if (is_down_pressed(keys_down)) {
        move_down(trophySelection);
    } else if (is_up_pressed(keys_down)) {
        move_up(trophySelection);
    }

    if (keys_down.c[0].B) {
        state = GAME_SELECT;
    }

    struct controller_data ckeys_held = get_keys_held();
    bool show_spoiler = ckeys_held.c[0].Z;

    int bronzeCount = 0;
    int silverCount = 0;
    int goldCount = 0;
    int percentageCompleted = 0;
    getGameStatus(game, &bronzeCount, &silverCount, &goldCount, &percentageCompleted);

    // Render

    // Header
    graphics_set_color(SCREEN_TITLE_COLOR, 0x0);
    graphics_draw_text(disp, 20, 15, game.title);

    graphics_set_color(TOTALS_COLOR, 0x0);
    draw_trophy_counter(disp, OFFSET_TROPHY_BRONZE_X, 10, bronzeCount, bronze);
    draw_trophy_counter(disp, OFFSET_TROPHY_SILVER_X, 10, silverCount, silver);
    draw_trophy_counter(disp, OFFSET_TROPHY_GOLD_X, 10, goldCount, gold);

    // Trophy list
    for (int i = trophySelection->startIndex; i < trophySelection->endIndex; i++) {
        bool is_selected = i == trophySelection->selectedIndex;
        draw_trophy(10, ((31 * (i - trophySelection->startIndex)) + HEADER_HEIGHT), disp, game.trophies[i],
                    is_selected, show_spoiler);
    }
}

void loadGameData(Game *game, char *saveGame, SaveType saveType, void (*f)(Game *, char *)) {
    FILE *saveState = fopen(saveGame, "r");

    char *save_data = malloc(get_size_of_save_data(saveType));
    if (saveState == NULL) {
        // Save is unavailable, just pass an empty buffer as the save file
        // TODO: This might cause invalid trophy states, lets create a better way to fix this
        debug_printf("Save data unavailable for %s\n", game->title);
        memset(save_data, 0, get_size_of_save_data(saveType));
    } else {
        // Read save file
        if (fread(save_data, get_size_of_save_data(saveType), 1, saveState)) {
            debug_printf("Read %s save game into memory size: %d\n", game->title, get_size_of_save_data(saveType));
        }
    }

    // Add and parse trophies
    (*f)(game, save_data);

    // Release save data and file
    free(save_data);
    fclose(saveState);
}

typedef struct {
    char game_path[257];
    char file_name[100];
} GamePath;

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

typedef struct {
    char dir[257];
} Dirs;

void
detect_games_in_dir(char *dir, SupportedGame *supported_games, int supported_game_count, DetectedGame *detected_games,
                    int *detected_game_count) {
    debug_printf("Reading dir '%s'\n", dir);
    Dirs subDirs[20];
    int subDirIndex = 0;

    int path_count = 0;
    GamePath paths[100];

    dir_t buf;
    int ret = dir_findfirst(dir, &buf);
    while (ret == 0 && buf.d_name[0] != '\0') {
        if (buf.d_type != DT_DIR) {
            // Check if this is an N64 rom
            char *dot = strrchr(buf.d_name, '.');
            if (dot && !strcmp(dot, ".z64") && buf.d_name[0] != '.') {
                debug_printf("Z64 file detected %s\n", buf.d_name);
                sprintf(paths[path_count].game_path, "%s/%s", dir, buf.d_name);
                strcpy(paths[path_count].file_name, strip_extension(buf.d_name));
                path_count++;
            }
        } else if (buf.d_name[0] != '.') { // Ignore hidden dirs
            if (subDirIndex < 20) {
                debug_printf("Detected dir '%s/%s'\n", dir, buf.d_name);
                sprintf(subDirs[subDirIndex++].dir, "%s/%s", dir, buf.d_name);
            } else {
                debug_printf("Reached maximum amount of subdirectories\n");
            }
        }
        ret = dir_findnext(dir, &buf);
    }
    debug_printf_and_pause("Pause\n");

    debug_printf("Found %d games... Detecting game type\n", path_count);
    char game_id[3];
    for (int i = 0; i < path_count; i++) {
        debug_printf("Identifying game %s\n", paths[i].game_path);
        // Checks the 'unique' identifier for this rom
        FILE *game_rom = fopen(paths[i].game_path, "r");
        if (game_rom == NULL) {
            debug_printf_and_pause("Unable to open game rom '%s'\n", paths[i].game_path);
            return;
        }
        // Game ID is at 60 (well 59 actually)
        fseek(game_rom, 60, SEEK_SET);
        fread(game_id, 3, 1, game_rom);
        fclose(game_rom);

        // Checks if the game is a supported game
        bool is_supported = false;
        for (int sg = 0; sg < supported_game_count; sg++) {
            if (strncmp(game_id, supported_games[sg].game_code, 3) == 0) {
                debug_printf("Detected %s\n", supported_games[sg].name);
                is_supported = true;

                strcpy(detected_games[*detected_game_count].filename, paths[i].file_name);
                detected_games[*detected_game_count].supported_game = supported_games[sg];
                *detected_game_count = *detected_game_count + 1;
            }
        }

        if (!is_supported) {
            debug_printf("Unknown game detected '%.3s' at %s\n", game_id, paths[i].game_path);
        }
    }

    // Scan subdirectories
    if (subDirIndex > 0) {
        for (int i = 0; i < subDirIndex; i++) {
            detect_games_in_dir(subDirs[i].dir, supported_games, supported_game_count, detected_games,
                                detected_game_count);
        }
    }
}

void detect_games(SupportedGame *supported_games, int supported_game_count, DetectedGame *detected_games,
                  int *detected_game_count) {
    debug_printf("Supported games:\n");
    for (int i = 0; i < supported_game_count; i++) {
        debug_printf("%s - %s\n", supported_games[i].game_code, supported_games[i].name);
    }

#ifdef N64_HARDWARE
    if (!debug_init_sdfs("sd:/", -1)) {
        debug_printf_and_stop("Error initializing SD (%s)\n", version);
    }
#endif

    debug_printf("Detecting games:\n");

#ifdef N64_HARDWARE
    detect_games_in_dir("sd:/", supported_games, supported_game_count, detected_games, detected_game_count);
#else
    detect_games_in_dir("rom://", supported_games, supported_game_count, detected_games, detected_game_count);
#endif
    debug_printf("Finished game detection\n");
}

void init_sprite_data() {
    int fp = dfs_open("/trophy_locked.sprite");
    locked = malloc(dfs_size(fp));
    dfs_read(locked, 1, dfs_size(fp), fp);
    dfs_close(fp);

    fp = dfs_open("/trophy_bronze.sprite");
    bronze = malloc(dfs_size(fp));
    dfs_read(bronze, 1, dfs_size(fp), fp);
    dfs_close(fp);

    fp = dfs_open("/trophy_silver.sprite");
    silver = malloc(dfs_size(fp));
    dfs_read(silver, 1, dfs_size(fp), fp);
    dfs_close(fp);

    fp = dfs_open("/trophy_gold.sprite");
    gold = malloc(dfs_size(fp));
    dfs_read(gold, 1, dfs_size(fp), fp);
    dfs_close(fp);

    fp = dfs_open("/trophy_splash.sprite");
    splash = malloc(dfs_size(fp));
    dfs_read(splash, 1, dfs_size(fp), fp);
    dfs_close(fp);

    fp = dfs_open("/button_a.sprite");
    button_a = malloc(dfs_size(fp));
    dfs_read(button_a, 1, dfs_size(fp), fp);
    dfs_close(fp);
}

void render_about(display_context_t disp) {
    graphics_draw_bordered_box(disp, 200, 200, 200, 200, SCREEN_BACKGROUND_COLOR, SCREEN_BACKGROUND_COLOR, 5);
}

int main(void) {
    display_init(RESOLUTION_640x480, DEPTH_32_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    if (dfs_init(DFS_DEFAULT_LOCATION) != DFS_ESUCCESS) {
        debug_printf_and_stop("Error initializing file system\n");
    }

    sprintf(about, "Shadow-Link - %s", version);

    console_init();
    controller_init();

    debug_init_usblog();
    console_set_debug(true);

    init_sprite_data();
    init_colors();
    init_supported_games();

    DetectedGame detected_games[100];
    int detected_game_count = 0;
    detect_games(supported_games, SUPPORTED_GAME_COUNT, detected_games, &detected_game_count);

    debug_printf("Detected games: %d\n", detected_game_count);

    debug_printf_and_pause("Loading trophy data\n");
    // TODO: Make this dynamic
    Game games[50];
    for (int i = 0; i < detected_game_count; i++) {
        debug_printf("Loading trophy data for %s\n", detected_games[i].filename);
        char save_path[512];
#ifdef N64_HARDWARE
        sprintf(save_path, "sd:/ED64/gamedata/%s.%s", detected_games[i].filename,
                get_extension_for_save_type(detected_games[i].supported_game.save_type));
#else
        sprintf(save_path, "rom://ED64/gamedata/%s.%s", detected_games[i].filename,
                get_extension_for_save_type(detected_games[i].supported_game.save_type));
#endif
        debug_printf("Loading game data '%s'\n", save_path);

        // Copy some info for display
        strcpy(games[i].title, detected_games[i].supported_game.name);
        strcpy(games[i].filename, detected_games[i].filename);
        games[i].region = detected_games[i].supported_game.region;

        games[i].trophyCount = 0;
        loadGameData(&games[i], save_path, detected_games[i].supported_game.save_type,
                     detected_games[i].supported_game.trophy_data_loader);
    }

    // Selection for game selection menu
    gameSelection = list_selection_new(6, detected_game_count);
    trophySelection = list_selection_new(6, 0);

    debug_printf_and_pause("Loaded game data\n");

    while (1) {
        /* Grab a render buffer */
        display_context_t disp = display_lock();

        /* Fill the screen */
        graphics_fill_screen(disp, SCREEN_BACKGROUND_COLOR);

        /* Render the screen */
        switch (state) {
            case ABOUT:
                render_about_screen(disp);
                break;
            case GAME_SELECT:
                render_game_select_screen(disp, games, detected_game_count);
                break;
            case TROPHY_OVERVIEW:
                render_trophy_screen(disp, *selectedGame);
                break;
        }

        render_footer(disp);

        /* Force backbuffer flip */
        display_show(disp);
    }
}
