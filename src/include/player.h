#pragma once

#include "../../res/map/tiled.h"
#include "animate.h"
#include "input.h"
#include "sprite.h"
#include "terrain_map.h"
#include "types.h"
#include "wasm4.h"
#include <stdint.h>

struct TerrainMap;

struct Player {
    uint8_t layer;

    struct WorldCoordinate loc;
    struct CharacterSprite sprite;
    enum Direction last_movement_dir;

    uint32_t player_height;
    uint32_t player_width;

    bool is_animating;
    bool is_sliding;
    struct Animation animation;
};

void handle_movement(struct Player *player,
                     const struct TerrainMap *terrain_map,
                     const struct InputState *inputs);

void draw_player(const struct Player *player);

struct BoundingBox player_make_bb(const struct Player *player);