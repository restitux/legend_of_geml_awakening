#include "block.h"
#include "../../res/map/tiled.h"

#include "../../res/data/block_sprite.h"

#include "configuration.h"

#include "input.h"
#include "player.h"
#include "sprite.h"
#include "types.h"

#include "room_renderer.h"

#include "wasm4.h"
//----------------------------------
//  CONFIGURATION
//---------------------------------
#define BLOCK_RAISED_SPRITE block_sprite
#define BLOCK_LOWERED_SPRITE

void block_new(struct GridCoordinate loc, struct Block *block) {
    *block = (struct Block){
        .layer = 0,
        .loc = loc,
        .draw_loc = coordinate_grid_to_screen(&loc),
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
                      enum Direction push_dir,
                      const struct TileMap_DataLayer *collision_map,
                      struct BlockPushAnimation *out) {

    struct GridCoordinate player_snap = block->loc;

    struct GridCoordinate target_loc = block->loc;

    if (push_dir == DIRECTION_UP) {
        target_loc.y -= 1;
        player_snap.y += 2;
    } else if (push_dir == DIRECTION_DOWN) {
        target_loc.y += 1;
        player_snap.y -= 2;
    } else if (push_dir == DIRECTION_LEFT) {
        target_loc.x -= 1;
        player_snap.x += 2;
    } else if (push_dir == DIRECTION_RIGHT) {
        target_loc.x += 1;
        player_snap.x -= 2;
    }

    player->loc.screen = coordinate_grid_to_screen(&player_snap);
    struct ScreenCoordinate block_target_screen_loc =
        coordinate_grid_to_screen(&target_loc);

    struct BoundingBox target_bb = bounding_box_new(
        block_target_screen_loc, BLOCK_SIZE - 1, BLOCK_SIZE - 1);

    ONLY_DEBUG(debug_bb_draw(&target_bb));

    uint8_t animation_time = 8 * BLOCK_FRAMES_PER_MOVE;
    if (room_is_tile_present_at_bb_corners(
            &target_bb, collision_map, player->loc.room, COLLISION_TYPE_ANY)) {
        // move is not valid
        animation_time = 0;
        target_loc = block->loc;
    }

    *out = (struct BlockPushAnimation){
        .player = player,
        .block = block,
        .target_loc = target_loc,
        .remainingFrames = animation_time,
        .dir = push_dir,
    };
}

void block_push_snap_player(struct BlockPushAnimation *push) {
    push->player->loc.screen = push->block->draw_loc;
    if (push->dir == DIRECTION_UP) {
        push->player->loc.screen.y += BLOCK_SIZE;
    } else if (push->dir == DIRECTION_DOWN) {
        push->player->loc.screen.y -= BLOCK_SIZE;
    } else if (push->dir == DIRECTION_LEFT) {
        push->player->loc.screen.x += BLOCK_SIZE;
    } else if (push->dir == DIRECTION_RIGHT) {
        push->player->loc.screen.x -= BLOCK_SIZE;
    }
}

void block_push_end(struct BlockPushAnimation *push) {
    push->block->loc = push->target_loc;
    block_push_snap_player(push);
}

bool block_push_step(struct BlockPushAnimation *push) {
    if (push->remainingFrames == 0) {
        block_push_end(push);
        return false;
    }

    if (push->remainingFrames % BLOCK_FRAMES_PER_MOVE == 0) {
        if (push->dir == DIRECTION_UP) {
            push->block->draw_loc.y -= 1;
            push->player->loc.screen.y += BLOCK_SIZE - 1;
        } else if (push->dir == DIRECTION_DOWN) {
            push->player->loc.screen.y -= BLOCK_SIZE + 1;
            push->block->draw_loc.y += 1;
        } else if (push->dir == DIRECTION_LEFT) {
            push->player->loc.screen.x += BLOCK_SIZE - 1;
            push->block->draw_loc.x -= 1;
        } else if (push->dir == DIRECTION_RIGHT) {
            push->player->loc.screen.x -= BLOCK_SIZE + 1;
            push->block->draw_loc.x += 1;
        }
    }

    block_push_snap_player(push);

    push->remainingFrames -= 1;
    return true;
}

void block_draw_block(struct Block *block) {
    if (block->layer == 0) {
        sprite_draw_sprite_frame(&block->raised_sprite, &block->draw_loc);

    } else {
        text("B", block->draw_loc.x, block->draw_loc.y);
        text("B", block->draw_loc.x + 8, block->draw_loc.y);
        text("B", block->draw_loc.x, block->draw_loc.y + 8);
        text("B", block->draw_loc.x + 8, block->draw_loc.y + 8);
    }
}

void block_update_layer(struct Block *b,
                        const struct TileMap_DataLayer *special_map,
                        struct RoomCoordinate room) {

    struct BoundingBox bb =
        bounding_box_new(b->draw_loc, BLOCK_SIZE - 1, BLOCK_SIZE - 1);

    int tile = room_is_tile_present_at_bb_corners(&bb, special_map, room,
                                                  COLLISION_TYPE_ALL);

    if (tile) {
        b->layer = 1; // fall to 1
    } else {
        b->layer = 0; // forced up to 0
    }
}

bool block_is_push_attempted(const struct Player *p, const struct Block *b,
                             const struct InputState *i, enum Direction *d) {
    struct BoundingBox box_bb =
        bounding_box_new(coordinate_grid_to_screen(&b->loc), 16, 16);
    bounding_box_uniform_shrink(&box_bb, BLOCK_BOUNDING_BOX_BUFFER);

    struct BoundingBox player_bb = bounding_box_new(p->loc.screen, 16, 16);

    ONLY_DEBUG(debug_bb_draw(&box_bb));
    ONLY_DEBUG(debug_bb_draw(&player_bb));
    if (bounding_box_intersect(&box_bb, &player_bb) && p->layer == b->layer) {

        enum Direction vertical;
        enum Direction horizontal;
        bool v = input_get_pressed_direction(i, INPUT_AXIS_VERTICAL, &vertical);
        bool h =
            input_get_pressed_direction(i, INPUT_AXIS_HORIZONTAL, &horizontal);

        if (v && h) {
            return false;
        }
        if (v) {
            *d = vertical;
            return true;
        }
        if (h) {
            *d = horizontal;
            return true;
        }
    }

    return false;
}