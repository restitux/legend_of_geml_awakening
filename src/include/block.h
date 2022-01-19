#pragma once

#include <stdbool.h>

#include "input.h"
#include "player.h"
#include "sprite.h"
#include "types.h"

// a block for puzzles which can be pushed by the player
struct Block {
  bool is_walkable; // player can path over block
  struct GridCoordinate loc;

  struct SpriteFrame raised_sprite;
  struct SpriteFrame lowered_sprite;
};

struct BlockPushAnimation {
  struct Player *player;
  struct Block *block;

  struct GridCoordinate target_loc;

  struct ScreenCoordinate block_screen_coordinate;

  uint8_t pixels_per_frame;

  uint8_t remainingFrames;

  enum Direction dir;
};

// instantiate a default block at the provided grid coordinates
void block_new(struct GridCoordinate loc, struct Block *block);

// start a block push. Aligns player to nearest grid
void block_push_begin(struct Player *player, struct Block *block,
                      enum Direction push_dir, struct BlockPushAnimation *out);

// step the block push. If animation is complete, returns false
bool block_push_step(struct BlockPushAnimation *push);

void block_draw_block_push(const struct BlockPushAnimation *push);

void block_draw_block_static(struct Block *block);

bool block_is_push_attempted(const struct Player *p, const struct Block *b,
                             const struct InputState *i, enum Direction *d);