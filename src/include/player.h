#pragma once

#include "../../res/map/tiled.h"
#include "input.h"
#include "sprite.h"
#include "types.h"
#include "wasm4.h"
#include <stdint.h>

struct Player {
    uint8_t layer;

    struct WorldCoordinate loc;
    struct CharacterSprite sprite;
    enum Direction last_movement_dir;

    uint32_t player_height;
    uint32_t player_width;
};

void handle_movement(struct Player *player, const struct TileMap *map,
                     const struct InputState *inputs);

void draw_player(const struct Player *player);
