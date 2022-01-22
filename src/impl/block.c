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
        // .loc = loc,
        // .draw_loc = coordinate_grid_to_screen(&loc),
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
        .movable = movable_object_new(coordinate_grid_to_screen(&loc),
                                      BLOCK_SIZE - 1, BLOCK_SIZE - 1),
    };
    // bounding_box_uniform_shrink(&block->movable.bb, 1);
}

bool block_is_terrain_passable(const struct Block *b, Terrain t) {
    enum TerrianType cur_layer = terrain_type(t, b->layer);
    enum TerrianType lower_layer = terrain_type(t, b->layer + 1);

    bool normal_ground = cur_layer == TERRAIN_NORMAL;
    bool ice_ground =
        (lower_layer == TERRAIN_SLIPPERY) && (cur_layer == TERRAIN_INVALID);

    bool pit_ground = (lower_layer == TERRAIN_NORMAL);

    bool block_ground =
        (lower_layer == TERRAIN_BLOCK) && (cur_layer == TERRAIN_INVALID);

    return normal_ground || ice_ground || pit_ground || block_ground;
}

bool block_move_target_valid(struct Block *b,
                             struct ScreenCoordinate target_loc,
                             const struct TerrainMap *terrain_map) {
    struct BoundingBox current_bb = b->movable.bb;
    struct BoundingBox target_bb =
        bounding_box_new(target_loc, BLOCK_SIZE - 1, BLOCK_SIZE - 1);

    struct ScreenCoordinate target_corners[4];
    bounding_box_corners(&target_bb, target_corners);

    bool valid_move = true;
    for (int i = 0; i < 4; i++) {
        if (!bounding_box_contains_point(&current_bb, target_corners[i])) {
            Terrain t = terrain_at_point(terrain_map, target_corners[i]);
            valid_move &= block_is_terrain_passable(b, t);
        }
    }

    return valid_move;
}

bool block_push_input_just_pressed(const struct InputState *i,
                                   enum Direction d) {
    switch (d) {
    case DIRECTION_UP:
        return i->button_up.justPressed;
    case DIRECTION_DOWN:
        return i->button_down.justPressed;
    case DIRECTION_LEFT:
        return i->button_left.justPressed;
    case DIRECTION_RIGHT:
        return i->button_right.justPressed;
    default:
        break;
    }
}

struct ScreenCoordinate block_compute_player_snap_location_for_interaction(
    struct ScreenCoordinate block_loc, enum Direction dir) {
    if (dir == DIRECTION_UP) {
        block_loc.y += (BLOCK_PLAYER_SNAP_DIST - 8 - 1);
    } else if (dir == DIRECTION_DOWN) {
        block_loc.y -= BLOCK_PLAYER_SNAP_DIST;
    } else if (dir == DIRECTION_LEFT) {
        block_loc.x += BLOCK_PLAYER_SNAP_DIST - 1;
    } else if (dir == DIRECTION_RIGHT) {
        block_loc.x -= BLOCK_PLAYER_SNAP_DIST;
    }
    return block_loc;
}

void block_player_snap_for_interaction(struct ScreenCoordinate block_loc,
                                       struct Player *player,
                                       enum Direction dir) {
    player->loc.screen =
        block_compute_player_snap_location_for_interaction(block_loc, dir);
}

void block_push_snap_player(struct BlockPushAnimation *push) {
    push->player->loc.screen = push->block->movable.bb.tl;
    if (push->dir == DIRECTION_UP) {
        push->player->loc.screen.y += (BLOCK_PLAYER_SNAP_DIST - 8 - 1);
    } else if (push->dir == DIRECTION_DOWN) {
        push->player->loc.screen.y -= BLOCK_PLAYER_SNAP_DIST;
    } else if (push->dir == DIRECTION_LEFT) {
        push->player->loc.screen.x += BLOCK_PLAYER_SNAP_DIST - 1;
    } else if (push->dir == DIRECTION_RIGHT) {
        push->player->loc.screen.x -= BLOCK_PLAYER_SNAP_DIST;
    }
}
void block_push_begin(struct Player *player, struct Block *block,
                      enum Direction push_dir,
                      const struct TerrainMap *terrain_map,
                      const struct InputState *i,
                      struct BlockPushAnimation *out) {

    struct GridCoordinate player_snap =
        coordinate_screen_to_grid(&block->movable.bb.tl);

    struct GridCoordinate target_loc = player_snap;

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

    // player->loc.screen = coordinate_grid_to_screen(&player_snap);
    struct ScreenCoordinate block_target_screen_loc =
        coordinate_grid_to_screen(&target_loc);

    struct BoundingBox target_bb = bounding_box_new(
        block_target_screen_loc, BLOCK_SIZE - 1, BLOCK_SIZE - 1);

    ONLY_DEBUG(debug_bb_draw(&target_bb));

    uint8_t animation_time = 8 * BLOCK_FRAMES_PER_MOVE * BLOCK_PUSH_DISTANCE;
    if (!block_move_target_valid(block, block_target_screen_loc, terrain_map) ||
        !block_push_input_just_pressed(i, push_dir)) {
        // move is not valid
        animation_time = 0;
        target_loc = coordinate_screen_to_grid(&block->movable.bb.tl);
    } else {
        block->movable.last_moved_dir = push_dir;
    }

    *out = (struct BlockPushAnimation){
        .player = player,
        .block = block,
        .target_loc = target_loc,
        .remainingFrames = animation_time,
        .dir = push_dir,
    };

    block_push_snap_player(out);
}

void block_push_end(struct BlockPushAnimation *push,
                    const struct TerrainMap *terrain_map) {
    push->block->movable.bb.tl = coordinate_grid_to_screen(&push->target_loc);
    // push->block->movable.bb.tl =
    // coordinate_grid_to_screen(&push->block->loc);
    block_push_snap_player(push);

    if (terrain_map) {
    }
    tracef("%d is checking slide", push->block->id);
    movable_check_slide(&push->block->movable, terrain_map);
}

bool block_push_step(struct BlockPushAnimation *push,
                     const struct TerrainMap *terrain_map) {
    if (push->remainingFrames == 0) {
        block_push_end(push, terrain_map);
        return false;
    }

    if (push->remainingFrames % BLOCK_FRAMES_PER_MOVE == 0) {
        if (push->dir == DIRECTION_UP) {
            push->block->movable.bb.tl.y -= 1;
        } else if (push->dir == DIRECTION_DOWN) {
            push->block->movable.bb.tl.y += 1;
        } else if (push->dir == DIRECTION_LEFT) {
            push->block->movable.bb.tl.x -= 1;
        } else if (push->dir == DIRECTION_RIGHT) {
            push->block->movable.bb.tl.x += 1;
        }
    }

    block_push_snap_player(push);

    push->block->movable.bb.tl = push->block->movable.bb.tl;

    push->remainingFrames -= 1;
    return true;
}

void block_draw_block(struct Block *block) {
#ifndef BLOCK_DEBUG_DRAW_ID
    if (block->layer == 0) {
        sprite_draw_sprite_frame(&block->raised_sprite, &block->movable.bb.tl);
    } else {
        sprite_draw_sprite_frame(&block->lowered_sprite, &block->movable.bb.tl);
    }
#else
    char str[3];
    str[1] = '0' + block->id % 10;
    str[0] = '0' + block->id / 10;

    str[2] = 0;

    text(str, block->movable.bb.tl.x, block->movable.bb.tl.y);
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
        if (main == TERRAIN_NORMAL || lower == TERRAIN_SLIPPERY) {
            return LAYER_MAIN;
        }
        return LAYER_LOWER;
    }
}

void block_update_layer(struct Block *b, const struct TerrainMap *terrain_map) {

    struct BoundingBox bb =
        bounding_box_new(b->movable.bb.tl, BLOCK_SIZE - 1, BLOCK_SIZE - 1);

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

    movable_update_slide(&b->movable, terrain_map);
}

bool block_is_push_attempted(const struct Player *p, const struct Block *b,
                             const struct InputState *i, enum Direction *d) {
    struct BoundingBox box_bb = b->movable.bb;
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

struct GridCoordinate block_grid_loc(const struct Block *b) {
    return coordinate_screen_to_grid(&b->movable.bb.tl);
}

bool is_player_attempting_iteraction(const struct Block *b,
                                     const struct Player *p,
                                     const struct InputState *i,
                                     enum Direction *d) {
    struct BoundingBox box_bb = b->movable.bb;
    bounding_box_uniform_shrink(
        &box_bb, BLOCK_BOUNDING_BOX_BUFFER); // the iteraction bounding box is a
                                             // bit smaller to make accidental
                                             // interaction harder

    struct BoundingBox player_bb = player_make_bb(p);

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

bool block_is_input_confirming_iteraction(const struct InputState *i,
                                          enum Direction d) {
    return block_push_input_just_pressed(i, d);
}

bool block_is_point_pushable(Terrain t) {
    enum TerrianType cur_layer = terrain_type(t, LAYER_MAIN);
    enum TerrianType lower_layer = terrain_type(t, LAYER_LOWER);

    bool normal_ground = cur_layer == TERRAIN_NORMAL;
    bool sunk_block =
        cur_layer == TERRAIN_INVALID && lower_layer == TERRAIN_BLOCK;

    bool pit =
        (cur_layer == TERRAIN_INVALID) && (lower_layer == TERRAIN_NORMAL);

    return normal_ground || sunk_block || pit;
}

bool block_is_point_slidable(Terrain t) {
    enum TerrianType cur_layer = terrain_type(t, LAYER_MAIN);
    enum TerrianType lower_layer = terrain_type(t, LAYER_LOWER);

    return (cur_layer == TERRAIN_INVALID) && (lower_layer == TERRAIN_SLIPPERY);
}

bool block_is_terrain_push_target(const struct BoundingBox *target_box,
                                  const struct TerrainMap *tm) {
    struct ScreenCoordinate corners[4];
    bounding_box_corners(target_box, corners);

    int passable_corners = 0;
    for (int i = 0; i < 4; i++) {
        Terrain t = terrain_at_point(tm, corners[i]);

        if (block_is_point_pushable(t))

        {
            passable_corners += 1;
        }
    }

    return passable_corners == 4;
}

bool block_is_terrain_slide_target(const struct BoundingBox *target_box,
                                   const struct TerrainMap *tm) {
    struct ScreenCoordinate corners[4];
    bounding_box_corners(target_box, corners);

    int passable_corners = 0;
    for (int i = 0; i < 4; i++) {
        Terrain t = terrain_at_point(tm, corners[i]);

        if (block_is_point_slidable(t))

        {
            passable_corners += 1;
        }
    }

    return passable_corners == 4;
}

void make_push_animations(struct Block *b, struct Player *p, enum Direction d,
                          struct ScreenCoordinate final_location) {
    struct Animation block_animation = {
        .animation_subject = &b->movable.bb.tl,
        .end_loc = final_location,
        .frame_per_move = 2,
        .frames_remaining = 2 * BLOCK_SIZE,
        .move_direction = d,
    };

    struct Animation player_animation = {
        .animation_subject = &p->loc.screen,
        .end_loc = block_compute_player_snap_location_for_interaction(
            final_location, d),
        .frame_per_move = 2,
        .frames_remaining = 2 * BLOCK_SIZE,
        .move_direction = d,
    };

    b->is_animating = true;
    b->animation = block_animation;

    p->is_animating = true;
    p->animation = player_animation;
}

void block_calculate_iteraction_result(struct Block *b, struct Player *p,
                                       enum Direction d,
                                       const struct TerrainMap *tm) {
    struct BoundingBox moved = b->movable.bb;
    moved.tl = coordinate_screen_add_direction(moved.tl, d, BLOCK_SIZE);

    if (block_is_terrain_push_target(&moved, tm)) {
        make_push_animations(b, p, d, moved.tl);
    } else if (block_is_terrain_slide_target(&moved, tm)) {
        // handle slide
    }
}

void block_perform_player_iteraction(struct Block *b, struct Player *p,
                                     enum Direction d,
                                     const struct InputState *i,
                                     const struct TerrainMap *tm) {

    block_player_snap_for_interaction(b->movable.bb.tl, p, d);
    if (block_is_input_confirming_iteraction(i, d)) {
        block_calculate_iteraction_result(b, p, d, tm);
    }
}

void block_handle_player_iteraction(struct Block *b, struct Player *p,
                                    const struct InputState *i,
                                    const struct TerrainMap *tm) {
    enum Direction d;
    if (is_player_attempting_iteraction(b, p, i, &d)) {
        block_perform_player_iteraction(b, p, d, i, tm);
    }
}

void block_update_all_blocks(struct Block *blocks, uint32_t size,
                             struct Player *p, const struct InputState *input,
                             const struct TerrainMap *tm) {
    for (uint32_t i = 0; i < size; i++) {
        struct Block *b = &blocks[i];
        if (b->is_animating) {
            b->is_animating = animtion_next_frame(&b->animation);
        } else {
            block_handle_player_iteraction(b, p, input, tm);
        }
        block_update_layer(&blocks[i], tm);
    }
}