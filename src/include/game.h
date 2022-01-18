#pragma once

#include "player.h"

struct GameState {
    struct Player player;

    const struct TileMap *overworld;
};

extern struct GameState game_state;
