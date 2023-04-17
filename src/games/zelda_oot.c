//
// Created by Shadow-Link on 16/04/2023.
//

#include <stdio.h>

#include "zelda_oot.h"

#include "../trophy.h"
#include "../game.h"
#include "../save_state_utils.h"

void get_trophy_data_zelda_oot(Game *game, FILE *saveState) {
    char save_data[32768];
    if (fread(&save_data, sizeof(save_data), 1, saveState)) {
        printf("Read Zelda OOT save game into memory\n");
    }

    // = Dungeons =
    // [X] Deku tree
    // [X] Dodongo
    // [X] Jabu jabu
    // [ ] Forest
    // [ ] Fire
    // [ ] Water
    // [ ] Spirit
    // [ ] Shadow
    // [ ] Ganons tower
    // = Events =
    // [X] Mido sword / shield
    // [ ] Saria ocarina
    // [ ] Ocarina of time
    // [ ] Move king zora
    // [ ] Got Epona
    // [ ] Meet princess zelda
    // [ ] Become an adult / Get master sword
    // [ ] Accepted by gerudo (entrance card)
    // [ ] Stone of agony ?
    // [ ] Make Darunia dance
    // [ ] Finished the game (as far as possible)
    // = Upgrades =
    // [ ] Got all shields
    // [ ] Big goron sword
    // [ ] Get hook shot
    // [ ] Biggest wallet
    // [ ] Biggest bomb bag
    // [ ] Biggest deku seed bag
    // [ ] Biggest arrow thingy
    // [ ] Blue tunic
    // [ ] Red tunic
    // [ ] Iron boots
    // [ ] Hover boots
    // = Collectables =
    // [X] First skulltala
    // [ ] All skulltalas (no cheats)
    // [ ] All heart pieces
    // [ ] All bottles
    // [ ] Double defence heart pieces
    // [ ] All magic spells
    // [ ] All magic meter upgrades
    // [ ] All songs
    // [ ] Saria song
    // [ ] Sun song
    // [ ] Song of storms
    // [ ] Eponas song
    // [ ] Zeldas lullaby (or does this clash with meet zelda)
    // [ ] All masks
    // [ ] Big poes
    // = Other =
    // [ ] Death count?
    // [ ] Play time?
    // [ ] Running man time?
    // [ ] Biggest fish (hyrule loach)
    // [ ] Beat all minigames (slingshot, bow and arrow, bombshu, ?)

    add_bool_trophy(game, "A wimp is still a wimp", "Get access to the Great Deku Tree", BRONZE,
                    raw_is_flag_set(save_data, 0xEF5, 0b00010000));
    add_bool_trophy(game, "Good...bye...", "Obtain the Kokiri Emerald", SILVER,
                    raw_is_flag_set(save_data, 0x0C6, 0b00000100));
    add_bool_trophy(game, "Sworn Brothers", "Obtain the Goron's Ruby", SILVER,
                    raw_is_flag_set(save_data, 0x0C6, 0b00001000));
    add_bool_trophy(game, "Don't tell my father...", "Obtain Zora's Sapphire", SILVER,
                    raw_is_flag_set(save_data, 0x0C6, 0b00010000));
    add_bool_trophy(game, "Creepy little buggers", "Collect your first Gold Skulltula", BRONZE,
                    raw_is_flag_set(save_data, 0x0C6, 0b10000000));
}
