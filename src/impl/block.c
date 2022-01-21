#include "block.h"

#include "../../res/map/tiled.h"

#include "../../res/data/block_lower_sprite.h"
#include "../../res/data/block_sprite.h"

#include "configuration.h"

#include "collision.h"
#include "input.h"
#include "player.h"
#include "sprite.h"
#include "types.h"

#include "wasm4.h"
//----------------------------------
//  CONFIGURATION
//---------------------------------
#define BLOCK_RAISED_SPRITE block_sprite
#define BLOCK_LOWERED_SPRITE block3

void block_new(struct GridCoordinate loc, struct Block *block) {
    *block = (struct Block){
        .layer = 0,
        .loc = loc,
        .draw_loc = coordinate_grid_to_screen(&loc),
        .raised_sprite =
            {
                .sprite_sheet = BLOCK_RAISED_SPRITE,
                .x = 0,
                .y = 0,
                .sheet_width = 16,
            },
        .lowered_sprite =
            {
                .sprite_sheet = BLOCK_LOWERED_SPRITE,
                .x = 0,
                .y = 0,
                .sheet_width = 16,
            },
    };
}

bool block_is_terrain_passable(const struct Block *b, Terrain t) {
    enum TerrianType cur_layer = terrain_type(t, b->layer);
    enum TerrianType lower_layer = terrain_type(t, b->layer + 1);

    return cur_layer == TERRAIN_NORMAL || lower_layer == TERRAIN_NORMAL ||
           lower_layer == TERRAIN_BLOCK;
}

bool block_move_target_valid(struct Block *b,
                             struct ScreenCoordinate target_loc,
                             const struct TerrainMap *terrain_map) {
    struct BoundingBox current_bb =
        bounding_box_new(b->draw_loc, BLOCK_SIZE, BLOCK_SIZE);
    struct BoundingBox target_bb =
        bounding_box_new(target_loc, BLOCK_SIZE, BLOCK_SIZE);
    bounding_box_uniform_shrink(&target_bb, 1);

    struct ScreenCoordinate target_corners[4];
    bounding_box_corners(&target_bb, target_corners);

    debug_bb_draw(&current_bb);
    debug_bb_draw(&target_bb);

    bool valid_move = true;
    for (int i = 0; i < 4; i++) {
        if (!bounding_box_contains_point(&current_bb, target_corners[i])) {
            Terrain t = terrain_at_point(terrain_map, target_corners[i]);
            valid_move &= block_is_terrain_passable(b, t);
        }
    }

    return valid_move;
}

void block_push_begin(struct Player *player, struct Block *block,
                      enum Direction push_dir,
                      const struct TerrainMap *terrain_map,
                      struct BlockPushAnimation *out) {

    struct GridCoordinate player_snap = block->loc;

    struct GridCoordinate target_loc = block->loc;

    if (push_dir == DIRECTION_UP) {
        target_loc.y -= BLOCK_PUSH_DISTANCE;
        player_snap.y += 2;
    } else if (push_dir == DIRECTION_DOWN) {
        target_loc.y += BLOCK_PUSH_DISTANCE;
        player_snap.y -= 2;
    } else if (push_dir == DIRECTION_LEFT) {
        target_loc.x -= BLOCK_PUSH_DISTANCE;
        player_snap.x += 2;
    } else if (push_dir == DIRECTION_RIGHT) {
        target_loc.x += BLOCK_PUSH_DISTANCE;
        player_snap.x -= 2;
    }

    player->loc.screen = coordinate_grid_to_screen(&player_snap);
    struct ScreenCoordinate block_target_screen_loc =
        coordinate_grid_to_screen(&target_loc);

    struct BoundingBox target_bb = bounding_box_new(
        block_target_screen_loc, BLOCK_SIZE - 1, BLOCK_SIZE - 1);

    ONLY_DEBUG(debug_bb_draw(&target_bb));

    uint8_t animation_time = 8 * BLOCK_FRAMES_PER_MOVE;
    if (!block_move_target_valid(block, block_target_screen_loc, terrain_map)) {
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
        push->player->loc.screen.y += BLOCK_SIZE - 8 - PLAYER_COLLISION_BUFFER;
    } else if (push->dir == DIRECTION_DOWN) {
        push->player->loc.screen.y -= BLOCK_SIZE - PLAYER_COLLISION_BUFFER;
    } else if (push->dir == DIRECTION_LEFT) {
        push->player->loc.screen.x += BLOCK_SIZE + PLAYER_COLLISION_BUFFER;
    } else if (push->dir == DIRECTION_RIGHT) {
        push->player->loc.screen.x -= BLOCK_SIZE - PLAYER_COLLISION_BUFFER;
    }
}

void block_push_end(struct BlockPushAnimation *push) {
    push->block->loc = push->target_loc;
    push->block->draw_loc = coordinate_grid_to_screen(&push->block->loc);
    block_push_snap_player(push);
}

bool block_push_step(struct BlockPushAnimation *push, struct InputState *i) {
    if (!input_any_dir_pressed(i) &&
        push->remainingFrames > ((8 * BLOCK_FRAMES_PER_MOVE) - 2)) {
        push->target_loc = push->block->loc;
        push->remainingFrames = 0; // cancel
    }
    if (push->remainingFrames == 0) {
        block_push_end(push);
        return false;
    }

    if (push->remainingFrames % BLOCK_FRAMES_PER_MOVE == 0) {
        if (push->dir == DIRECTION_UP) {
            push->block->draw_loc.y -= 1;
        } else if (push->dir == DIRECTION_DOWN) {
            push->block->draw_loc.y += 1;
        } else if (push->dir == DIRECTION_LEFT) {
            push->block->draw_loc.x -= 1;
        } else if (push->dir == DIRECTION_RIGHT) {
            push->block->draw_loc.x += 1;
        }
    }

    block_push_snap_player(push);

    push->remainingFrames -= 1;
    return true;
}

void block_draw_block(struct Block *block) {
#ifndef BLOCK_DEBUG_DRAW_ID
    if (block->layer == 0) {
        sprite_draw_sprite_frame(&block->raised_sprite, &block->draw_loc);
    } else {
        sprite_draw_sprite_frame(&block->lowered_sprite, &block->draw_loc);
    }
#else
    char str[3];
    str[1] = '0' + block->id % 10;
    str[0] = '0' + block->id / 10;

    str[2] = 0;

    text(str, block->draw_loc.x, block->draw_loc.y);
#endif
}

void block_draw_all_blocks(struct Block *blocks, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        if (blocks[i].layer == LAYER_LOWER) {
            block_draw_block(&blocks[i]);
        }
    }
    for (uint32_t i = 0; i < size; i++) {
        if (blocks[i].layer == LAYER_MAIN) {
            block_draw_block(&blocks[i]);
        }
    }
}

int block_decide_layer(Terrain t, int cur_layer) {
    enum TerrianType main = terrain_type(t, LAYER_MAIN);
    enum TerrianType lower = terrain_type(t, LAYER_LOWER);

    if (cur_layer == LAYER_MAIN) {
        if ((main == TERRAIN_INVALID || main == TERRAIN_BLOCK)) {
            if (lower == TERRAIN_NORMAL) {
                return LAYER_LOWER;
            }
        }
        return LAYER_MAIN;
    } else {
        if (main == TERRAIN_NORMAL) {
            return LAYER_MAIN;
        }
        return LAYER_LOWER;
    }
}

void block_update_layer(struct Block *b, const struct TerrainMap *terrain_map) {

    struct BoundingBox bb =
        bounding_box_new(b->draw_loc, BLOCK_SIZE - 1, BLOCK_SIZE - 1);

    struct ScreenCoordinate corners[4];
    bounding_box_corners(&bb, corners);
    int layer = 0;
    for (int i = 0; i < 4; i++) {
        layer += block_decide_layer(terrain_at_point(terrain_map, corners[i]),
                                    b->layer);
    }

    if (layer == 4) {
        b->layer = 1;
    } else if (layer == 0) {
        b->layer = 0;
    }
}

bool block_is_push_attempted(const struct Player *p, const struct Block *b,
                             const struct InputState *i, enum Direction *d) {
    struct BoundingBox box_bb =
        bounding_box_new(coordinate_grid_to_screen(&b->loc), 16, 16);
    bounding_box_uniform_shrink(&box_bb, BLOCK_BOUNDING_BOX_BUFFER);

    struct BoundingBox player_bb = player_make_bb(p);

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