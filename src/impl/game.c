#include "game.h"
#include "../../res/data/player_animation.h"
#include "../../res/map/dungeon_one.map.h"
#include "../../res/map/testing.map.h"
#include "../../res/map/tiled.h"
#include "wasm4.h"

struct GameState game_state;
void init_game() {
  game_state = (struct GameState){
      .valid = true,
      .player = {.loc = {.room = {0, 0}, .screen = {35, 70}},
                 .sprite = {.sprite_sheet = player_animation,
                            .sheet_height = player_animationHeight,
                            .sheet_width = player_animationWidth,
                            .frame_size = player_animationFrameSize,
                            .current_frame = 0,
                            .frames_per_animation = 10},
                 .player_height = 16,
                 .player_width = 16},
      .overworld = &testing_tilemap,
  };
}

void save_game() { diskw(&game_state, sizeof(struct GameState)); }

void load_game() {
  struct GameState temp;
  diskr(&temp, sizeof(struct GameState));
  if (temp.valid) {
    game_state = temp;
  }
}
