#pragma once

#include <stdbool.h>

#include "input.h"
#include "player.h"
#include "sprite.h"
#include "terrain_map.h"
#include "types.h"

struct TerrainMap;

// a block for puzzles which can be pushed by the player
struct Block {
    uint16_t id;
    uint8_t layer;
    struct GridCoordinate loc;
    struct ScreenCoordinate draw_loc;

    struct SpriteFrame raised_sprite;
    struct SpriteFrame lowered_sprite;
};

struct BlockPushAnimation {
    struct Player *player;
    struct Block *block;

    struct GridCoordinate target_loc;

    uint8_t pixels_per_frame;

    uint8_t remainingFrames;

    enum Direction dir;
};

// instantiate a default block at the provided grid coordinates
void block_new(struct GridCoordinate loc, struct Block *block);

// start a block push. Aligns player to nearest grid
void block_push_begin(struct Player *player, struct Block *block,
                      enum Direction push_dir,
                      const struct TerrainMap *terrain_map,
                      struct BlockPushAnimation *out);

// step the block push. If animation is complete, returns false
bool block_push_step(struct BlockPushAnimation *push, struct InputState *i);

void block_draw_block(struct Block *block);

bool block_is_push_attempted(const struct Player *p, const struct Block *b,
                             const struct InputState *i, enum Direction *d);

void block_update_layer(struct Block *b, const struct TerrainMap *terrain_map);