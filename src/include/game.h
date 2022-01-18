#pragma once

#include "input.h"
#include "player.h"
#include "room_renderer.h"

struct GameState {
  bool valid;
  struct Player player;

  const struct TileMap *overworld;

  struct Room currentRoom;

  struct InputState inputs;
};

extern struct GameState game_state;

void init_game();

void save_game();

void load_game();