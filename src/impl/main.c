#include "game.h" //main must include game.h for the state to be initalized
#include "hooks.h"

#include "../../res/map/dungeon_one.map.h"
#include "../../res/map/dungeon_two.map.h"
#include "../../res/map/testing.map.h"
#include "block.h"
#include "save.h"

#include "wasm4.h"

void start() {
    initalize_dungeon_one_tilemap();
    initalize_dungeon_two_tilemap();
    initalize_testing_tilemap();
    init_game();

    // if (load_save_game()) {
    //     trace("game loaded");
    // }

    on_game_launch();
}

void update() { on_update(); }
