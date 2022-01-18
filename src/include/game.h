#pragma once

#include "player.h"

struct GameState {
  bool valid;
  struct Player player;

  const struct TileMap *overworld;
};

extern struct GameState game_state;

void init_game();

void save_game();

void load_game();