#pragma once

#include <stdbool.h>

#include "animate.h"
#include "input.h"
#include "movable.h"
#include "player.h"
#include "sprite.h"
#include "terrain_map.h"
#include "types.h"
// a block for puzzles which can be pushed by the player
struct Block {
    uint16_t id;
    uint8_t layer;
    // struct GridCoordinate loc;
    // struct ScreenCoordinate draw_loc;

    struct SpriteFrame raised_sprite;
    struct SpriteFrame lowered_sprite;

    struct MovableObject movable;

    bool is_animating;
    struct Animation animation;
};

// instantiate a default block at the provided grid coordinates
void block_new(struct GridCoordinate loc, struct Block *block);

void block_draw_block(struct Block *block);

void block_update_layer(struct Block *b, const struct TerrainMap *terrain_map);

void block_draw_all_blocks(struct Block *blocks, uint32_t size);

struct GridCoordinate block_grid_loc(const struct Block *b);

void block_handle_player_iteraction(struct Block *b, struct Player *p,
                                    const struct InputState *i,
                                    const struct TerrainMap *tm);

void block_update_all_blocks(struct Block *blocks, uint32_t size,
                             struct Player *p, const struct InputState *i,
                             const struct TerrainMap *tm);

void block_set_layer(struct Block *blocks, enum TerrainLayer layer);