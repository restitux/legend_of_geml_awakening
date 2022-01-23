#include "player.h"
#include "../../res/map/tiled.h"
#include "collision.h"
#include "configuration.h"
#include "hooks.h"
#include "room_renderer.h"
#include "sprite.h"
#include "types.h"
#include "wasm4.h"

struct BoundingBox player_make_bb(const struct Player *player) {
    struct BoundingBox bb =
        bounding_box_new(player->loc.screen, PLAYER_SIZE, PLAYER_SIZE / 2);
    bounding_box_uniform_shrink(&bb, PLAYER_COLLISION_BUFFER);

    bb.tl.y += PLAYER_SIZE / 2;
    return bb;
}

void player_make_potential_move(struct BoundingBox *moved,
                                struct WorldCoordinate *potential,
                                enum Direction direction, uint8_t dist) {
    switch (direction) {
    case DIRECTION_UP:
        (*moved).tl.y -= dist;
        (*potential).screen.y -= dist;
        break;

    case DIRECTION_DOWN:
        (*moved).tl.y += dist;
        (*potential).screen.y += dist;
        break;

    case DIRECTION_LEFT:
        (*moved).tl.x -= dist;
        (*potential).screen.x -= dist;
        break;

    case DIRECTION_RIGHT:
        (*moved).tl.x += dist;
        (*potential).screen.x += dist;
        break;
    default:
        break;
    }
}

bool player_is_point_walkable(Terrain t) {
    enum TerrianType cur_layer = terrain_type(t, LAYER_MAIN);
    enum TerrianType lower_layer = terrain_type(t, LAYER_LOWER);

    bool not_wall =
        (cur_layer != TERRAIN_WALL && lower_layer == TERRAIN_INVALID);
    bool sunk_block = (lower_layer == TERRAIN_BLOCK);
    // bool ice_with_block = (lower_layer == TERRAIN_SLIPPERY);
    return not_wall || sunk_block;
}

bool player_is_point_walkable_or_ice(Terrain t) {
    enum TerrianType cur_layer = terrain_type(t, LAYER_MAIN);
    enum TerrianType lower_layer = terrain_type(t, LAYER_LOWER);

    bool not_wall =
        (cur_layer != TERRAIN_WALL && lower_layer == TERRAIN_INVALID);
    bool sunk_block = (lower_layer == TERRAIN_BLOCK);
    bool ice =
        (cur_layer == TERRAIN_INVALID && lower_layer == TERRAIN_SLIPPERY);
    return not_wall || sunk_block || ice;
}

bool player_is_point_slideover(Terrain t) {
    enum TerrianType cur_layer = terrain_type(t, LAYER_MAIN);
    enum TerrianType lower_layer = terrain_type(t, LAYER_LOWER);

    bool not_wall =
        (cur_layer == TERRAIN_NORMAL && lower_layer == TERRAIN_INVALID);
    bool sunk_block =
        (cur_layer == TERRAIN_INVALID && lower_layer == TERRAIN_BLOCK);
    return not_wall || sunk_block;
}

bool player_is_point_slide(Terrain t) {
    enum TerrianType cur_layer = terrain_type(t, LAYER_MAIN);
    enum TerrianType lower_layer = terrain_type(t, LAYER_LOWER);

    bool ice =
        (cur_layer == TERRAIN_INVALID && lower_layer == TERRAIN_SLIPPERY);

    return ice;
}

void player_do_slide(struct BoundingBox slide_check, struct Player *player,
                     const struct TerrainMap *terrain_map,
                     enum Direction direction) {
    slide_check = player_make_bb(player);

    debug_bb_draw(&slide_check);

    uint8_t start_move_amount = GRID_SIZE;
    uint8_t step_size = GRID_SIZE;
    uint8_t slide_past_steps = 1;
    if (DIRECTION_LEFT == direction || DIRECTION_RIGHT == direction) {
        slide_past_steps = 2; // deal with collision box size issues
    }
    if (DIRECTION_LEFT == direction) {
        slide_check.width -= GRID_SIZE;
    }
    if (DIRECTION_RIGHT == direction) {
        slide_check.width -= GRID_SIZE;
        slide_check.tl.x += GRID_SIZE;
    }

    slide_check.tl = coordinate_screen_add_direction(slide_check.tl, direction,
                                                     start_move_amount);
    if (terrain_is_check_all_target(&slide_check, terrain_map,
                                    player_is_point_slide)) {

        uint8_t slide_distance = terrain_calc_slide_distance(
            slide_check, direction, terrain_map, step_size, slide_past_steps,
            terrain_is_point_slidable, player_is_point_slideover);

        player->is_animating = true;
        player->is_sliding = true;

        struct ScreenCoordinate target_top_left = slide_check.tl;
        target_top_left.y -= (8);
        // slide_check.tl.y -= 8;
        coordinate_align_to_grid(&target_top_left);

        if (direction == DIRECTION_RIGHT) {
            target_top_left.x -= GRID_SIZE;
        }

        if (slide_distance >= GRID_SIZE) {
            slide_distance -= GRID_SIZE;
        }

        player->animation = (struct Animation){
            .animation_subject = &(player->loc.screen),
            .end_loc = coordinate_screen_add_direction(
                target_top_left, direction, slide_distance),
            .frame_per_move = 1,
            .frames_remaining = slide_distance,
            .move_direction = direction,
        };
    }
}

void move_player_if_valid(struct Player *player, enum Direction direction,

                          const struct TerrainMap *terrain_map, uint8_t dist) {

    struct BoundingBox bb = player_make_bb(player);

    player_do_slide(bb, player, terrain_map, direction);
    if (player->is_animating) {
        return;
    }

    TerrainPointCheck walkable_check = player_is_point_walkable;

    if (terrain_is_check_all_target(&bb, terrain_map,
                                    terrain_is_point_slidable)) {
        struct ScreenCoordinate sample_loc =
            coordinate_screen_add_direction(bb.tl, direction, 16);

        Terrain t = terrain_at_point(terrain_map, sample_loc);
        if (player_is_point_walkable(t)) {
            walkable_check = player_is_point_walkable_or_ice;
            dist = 16;
        }
    }

    struct WorldCoordinate potential_loc = player->loc;
    player_make_potential_move(&bb, &potential_loc, direction, dist);

    bool can_pass =
        terrain_is_check_all_target(&bb, terrain_map, walkable_check);

    if (can_pass) {
        if (potential_loc.screen.y < player->loc.screen.y) {
            player->last_movement_dir = DIRECTION_UP;
        } else if (potential_loc.screen.y > player->loc.screen.y) {
            player->last_movement_dir = DIRECTION_DOWN;
        }

        if (potential_loc.screen.x < player->loc.screen.x) {
            player->last_movement_dir = DIRECTION_LEFT;
        } else if (potential_loc.screen.x > player->loc.screen.x) {
            player->last_movement_dir = DIRECTION_RIGHT;
        }

        player->loc = potential_loc;
        sprite_advance_animation(&player->sprite);
    }
}

bool check_room_change(struct Player *player) {
    bool room_change = false;
    if (player->loc.screen.x > 159) {
        player->loc.screen.x = 1;
        player->loc.room.x += 1;
        room_change = true;
    }
    if (player->loc.screen.x < 1) {
        player->loc.screen.x = 159;
        player->loc.room.x -= 1;
        room_change = true;
    }
    if (player->loc.screen.y > 159) {
        player->loc.screen.y = 1;
        player->loc.room.y += 1;
        room_change = true;
    }
    if (player->loc.screen.y < 1) {
        player->loc.screen.y = 159;
        player->loc.room.y -= 1;
        room_change = true;
    }
    return room_change;
}

bool player_point_is_block(Terrain t) {
    return terrain_type(t, LAYER_MAIN) == TERRAIN_BLOCK;
}

void handle_movement(struct Player *player,
                     const struct TerrainMap *terrain_map,
                     const struct InputState *inputs) {
    if (player->is_animating) {
        player->last_movement_dir = player->animation.move_direction;
        player->is_animating = animtion_next_frame(&player->animation);

        struct BoundingBox target_bb = bounding_box_new(
            player->animation.end_loc, PLAYER_SIZE - 1, (PLAYER_SIZE - 1));
        if ((terrain_is_check_corners(&target_bb, terrain_map,
                                      player_point_is_block) > 2) &&
            player->is_sliding) {
            player->animation.end_loc = coordinate_screen_add_direction(
                player->animation.end_loc,
                direction_reverse(player->animation.move_direction), GRID_SIZE);
            if (player->animation.frames_remaining >= GRID_SIZE) {
                player->animation.frames_remaining -= GRID_SIZE;
            }
        }

        if (!player->is_animating && player->is_sliding) {
            player->is_sliding = false;
        }

        return;
    }
    struct BoundingBox bb = player_make_bb(player);

    enum Direction d;
    if (input_get_pressed_direction(inputs, INPUT_AXIS_VERTICAL, &d)) {
        move_player_if_valid(player, d, terrain_map, 1);
    }
    if (input_get_pressed_direction(inputs, INPUT_AXIS_HORIZONTAL, &d) &&
        !player->is_animating) {
        move_player_if_valid(player, d, terrain_map, 1);
    }

    if (check_room_change(player)) {
        on_room_exit();
        on_room_enter();
    }
}

void draw_player(const struct Player *player) {

    sprite_draw_character(&player->sprite, &player->loc.screen,
                          player->last_movement_dir);
}
