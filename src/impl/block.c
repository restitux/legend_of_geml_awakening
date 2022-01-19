#include "block.h"
#include "../../res/data/block_sprite.h"

#include "configuration.h"

#include "input.h"
#include "player.h"
#include "sprite.h"
#include "types.h"

#include "wasm4.h"
//----------------------------------
//  CONFIGURATION
//---------------------------------
#define BLOCK_RAISED_SPRITE block_sprite
#define BLOCK_LOWERED_SPRITE

void block_new(struct GridCoordinate loc, struct Block *block) {
  *block = (struct Block){
      .is_walkable = BLOCK_WALKABLE_ON_CREATE,
      .loc = loc,
      .raised_sprite =
          {
              .sprite_sheet = block_sprite,
              .x = 0,
              .y = 0,
              .sheet_width = 16,
          },
  };
}

void block_push_begin(struct Player *player, struct Block *block,
                      enum Direction push_dir, struct BlockPushAnimation *out) {

  struct GridCoordinate player_snap = block->loc;

  struct GridCoordinate target_loc = block->loc;

  if (push_dir == DIRECTION_UP) {
    target_loc.y -= 1;
    player_snap.y += 1;
  } else if (push_dir == DIRECTION_DOWN) {
    target_loc.y += 1;
    player_snap.y -= 1;
  } else if (push_dir == DIRECTION_LEFT) {
    target_loc.x -= 1;
    player_snap.x += 1;
  } else if (push_dir == DIRECTION_RIGHT) {
    target_loc.y += 1;
    player_snap.x -= 1;
  }

  player->loc.screen = coordinate_grid_to_screen(&player_snap);

  *out = (struct BlockPushAnimation){
      .player = player,
      .block = block,
      .target_loc = target_loc,
      .block_screen_coordinate = coordinate_grid_to_screen(&block->loc),
      .remainingFrames = 8 * FRAMES_PER_MOVE,
      .dir = push_dir,
  };
}

void block_push_end(struct BlockPushAnimation *push) {
  push->block->loc = push->target_loc;
}

bool block_push_step(struct BlockPushAnimation *push) {
  if (push->remainingFrames == 0) {
    block_push_end(push);
    return false;
  }

  if (push->remainingFrames % FRAMES_PER_MOVE == 0) {
    if (push->dir == DIRECTION_UP) {
      push->block_screen_coordinate.y -= 1;
      push->player->loc.screen.y += BLOCK_SIZE - 1;
    } else if (push->dir == DIRECTION_DOWN) {
      push->player->loc.screen.y -= BLOCK_SIZE + 1;
      push->block_screen_coordinate.y += 1;
    } else if (push->dir == DIRECTION_LEFT) {
      push->player->loc.screen.x += BLOCK_SIZE - 1;
      push->block_screen_coordinate.x -= 1;
    } else if (push->dir == DIRECTION_RIGHT) {
      push->player->loc.screen.x -= BLOCK_SIZE + 1;
      push->block_screen_coordinate.x += 1;
    }
  }

  push->player->loc.screen = push->block_screen_coordinate;
  if (push->dir == DIRECTION_UP) {
    push->player->loc.screen.y += BLOCK_SIZE;
  } else if (push->dir == DIRECTION_DOWN) {
    push->player->loc.screen.y -= BLOCK_SIZE;
  } else if (push->dir == DIRECTION_LEFT) {
    push->player->loc.screen.x += BLOCK_SIZE;
  } else if (push->dir == DIRECTION_RIGHT) {
    push->player->loc.screen.x -= BLOCK_SIZE;
  }

  push->remainingFrames -= 1;
  return true;
}

void block_draw_block_push(const struct BlockPushAnimation *push) {
  // draw_player(push->player);

  sprite_draw_sprite_frame(&push->block->raised_sprite,
                           &push->block_screen_coordinate);
}

void block_draw_block_static(struct Block *block) {
  struct ScreenCoordinate s = coordinate_grid_to_screen(&block->loc);
  // text("B", s.x, s.y);
  // blit(block->raised_sprite.sprite_sheet, s.x, s.y, 16, 16, BLIT_2BPP);
  sprite_draw_sprite_frame(&block->raised_sprite, &s);
}

bool block_is_push_attempted(const struct Player *p, const struct Block *b,
                             const struct InputState *i, enum Direction *d) {
  struct BoundingBox box_bb =
      bounding_box_new(coordinate_grid_to_screen(&b->loc), 16, 16);

  struct BoundingBox player_bb = bounding_box_new(p->loc.screen, 16, 16);

  ONLY_DEBUG(debug_bb_draw(&box_bb));
  ONLY_DEBUG(debug_bb_draw(&player_bb));
  if (bounding_box_intersect(&box_bb, &player_bb) && input_any_dir_pressed(i)) {
    if (i->button_up.isPressed) {
      *d = DIRECTION_UP;
    }
    if (i->button_down.isPressed) {
      *d = DIRECTION_DOWN;
    }
    if (i->button_left.isPressed) {
      *d = DIRECTION_LEFT;
    }
    if (i->button_right.isPressed) {
      *d = DIRECTION_RIGHT;
    }
    return true;
  }

  return false;
  // struct S
}