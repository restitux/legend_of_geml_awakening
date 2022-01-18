#include "game.h" //main must include game.h for the state to be initalized
#include "hooks.h"

#include "../../res/map/dungeon_one.map.h"
#include "../../res/map/testing.map.h"

#include "block.h"

void start() {
  initalize_dungeon_one_tilemap();
  initalize_testing_tilemap();
  init_game();

  on_game_launch();
}

void update() { on_update(); }
