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
#include "games/1080_snowboarding.h"
#include "games/super_mario_64.h"
#include "games/super_smash_bros.h"
#include "games/mario_kart_64.h"
#include "games/zelda_oot.h"

typedef enum {
    LOADING,
    GAME_SELECT,
    TROPHY_OVERVIEW
} State;

State state = GAME_SELECT;
Game *selectedGame;

sprite_t *silhouette;
sprite_t *bronze;
sprite_t *silver;
sprite_t *gold;

ListSelection *gameSelection;
ListSelection *trophySelection;

const char *spoilerDescription = "Trophy contains spoilers";

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
                return silhouette;
        }
    } else {
        return silhouette;
    }
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
    graphics_set_color(tile_text_color, 0x0);
    graphics_draw_bordered_box(disp, x, y, 620, 30, tile_background_color, tile_border_color, BORDER_THICKNESS);
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

void render_game_select_screen(display_context_t disp, Game *games, int gameCount) {
    // Check controller input
    controller_scan();
    struct controller_data keys_down = get_keys_down();

    if (is_down_pressed(keys_down)) {
        move_down(gameSelection);
    } else if (is_up_pressed(keys_down)) {
        move_up(gameSelection);
    }

    if (keys_down.c[0].A) {
        on_game_selected(&games[gameSelection->selectedIndex]);
    }

    // Render

    // Header
    graphics_set_color(SCREEN_TITLE_COLOR, 0x0);
    graphics_draw_text(disp, 20, 15, "N64 Trophies");

    // Totals
    int bronzeCount = 0, silverCount = 0, goldCount = 0, completedCount = 0;
    get_trophy_totals(games, gameCount, &bronzeCount, &silverCount, &goldCount, &completedCount);
    graphics_set_color(TOTALS_COLOR, 0x0);
    draw_trophy_counter(disp, OFFSET_TROPHY_BRONZE_X, 10, bronzeCount, bronze);
    draw_trophy_counter(disp, OFFSET_TROPHY_SILVER_X, 10, silverCount, silver);
    draw_trophy_counter(disp, OFFSET_TROPHY_GOLD_X, 10, goldCount, gold);

    // Game list
    for (int i = gameSelection->startIndex; i < gameSelection->endIndex; i++) {
        bool is_selected = i == gameSelection->selectedIndex;
        draw_game_tile(disp, 10, (i - gameSelection->startIndex) * 31 + HEADER_HEIGHT, games[i], is_selected);
    }
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
        draw_trophy(10, ((30 * (i - trophySelection->startIndex)) + HEADER_HEIGHT), disp, game.trophies[i],
                    is_selected, show_spoiler);
    }
}

void loadGameData(Game *game, char *saveGame, SaveType saveType, void (*f)(Game *, char *)) {
    FILE *saveState = fopen(saveGame, "r");
    if (saveState == NULL) {
        debug_printf("Save data unavailable for %s\n", game->title);
        return;
    }

    // Read save file
    char *save_data = malloc(get_size_of_save_data(saveType));
    if (fread(save_data, get_size_of_save_data(saveType), 1, saveState)) {
        debug_printf("Read %s save game into memory size: %d\n", game->title, get_size_of_save_data(saveType));
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

typedef void (*TrophyDataLoader)(Game *, char *);

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
    debug_printf("Supported games:\n");
    for (int i = 0; i < supported_game_count; i++) {
        debug_printf("%s - %s\n", supported_games[i].game_code, supported_games[i].name);
    }

    debug_printf("Detecting games:\n");
    int path_count = 0;
    GamePath paths[10];
//    int counter = 0;

    dir_t buf;
    int ret = dir_findfirst(dir, &buf);
    while (ret == 0 && buf.d_name[0] != '\0') {
        if (buf.d_type != DT_DIR) {
            // Check if this is an N64 rom
            char *dot = strrchr(buf.d_name, '.');
            if (dot && !strcmp(dot, ".z64") && buf.d_name[0] != '.') {
                debug_printf("Z64 file detected %s\n", buf.d_name);
                sprintf(paths[path_count].game_path, "%s%s", dir, buf.d_name);
                strcpy(paths[path_count].file_name, strip_extension(buf.d_name));
                path_count++;
            }
        }
        ret = dir_findnext(dir, &buf);
    }

    debug_printf("Found %d games... Detecting game type\n", path_count);
    char game_id[3];
    for (int i = 0; i < path_count; i++) {
        debug_printf("Detecting game %s\n", paths[i].game_path);
        // Checks the 'unique' identifier for this rom
        FILE *game_rom = fopen(paths[i].game_path, "r");
//        debug_printf("Opened game %s\n", paths[i].game_path);
        if (game_rom == NULL) {
            debug_printf_and_stop("Unable to open game rom");
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

        if (is_supported) {
            debug_printf("Unknown game detected '%.3s' at %s \n", game_id, paths[i].game_path);
        }
    }
    debug_printf("Finished game detection\n");
}

void detect_games(SupportedGame *supported_games, int supported_game_count, DetectedGame *detected_games,
                  int *detected_game_count) {
#ifdef N64_HARDWARE
    if (!debug_init_sdfs("sd:/", -1)) {
#else
    if (dfs_init(DFS_DEFAULT_LOCATION) != DFS_ESUCCESS) {
#endif
        debug_printf_and_stop("Error opening SD");
    }

#ifdef N64_HARDWARE
    print_dir("sd:/", supported_games, supported_game_count, detected_games, detected_game_count);
#else
    print_dir("rom://", supported_games, supported_game_count, detected_games, detected_game_count);
#endif
}

void load_sprite_data() {
    int fp = dfs_open("/trophy_silhouette.sprite");
    silhouette = malloc(dfs_size(fp));
    dfs_read(silhouette, 1, dfs_size(fp), fp);
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
}

int main(void) {
    display_init(RESOLUTION_640x480, DEPTH_32_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);

    console_init();
    controller_init();

    debug_init_usblog();
    console_set_debug(true);

    load_sprite_data();
    init_colors();

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

    debug_printf("Detected games: %d\n", detected_game_count);

    debug_printf_and_pause("Loading trophy data\n");
    // TODO: Make this dynamic
    Game games[6];
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

        strcpy(games[i].title, detected_games[i].supported_game.name);
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
        if (state == GAME_SELECT) {
            render_game_select_screen(disp, games, detected_game_count);
        } else if (state == TROPHY_OVERVIEW) {
            render_trophy_screen(disp, *selectedGame);
        }

        /* Force backbuffer flip */
        display_show(disp);
    }
}
