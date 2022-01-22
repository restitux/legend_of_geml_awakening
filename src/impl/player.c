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
                                enum Direction direction) {
    switch (direction) {
    case DIRECTION_UP:
        (*moved).tl.y -= 1;
        (*potential).screen.y -= 1;
        break;

    case DIRECTION_DOWN:
        (*moved).tl.y += 1;
        (*potential).screen.y += 1;
        break;

    case DIRECTION_LEFT:
        (*moved).tl.x -= 1;
        (*potential).screen.x -= 1;
        break;

    case DIRECTION_RIGHT:
        (*moved).tl.x += 1;
        (*potential).screen.x += 1;
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
    bool sunk_block =
        (cur_layer == TERRAIN_INVALID && lower_layer == TERRAIN_BLOCK);
    return not_wall || sunk_block;
}

void move_player_if_valid(struct Player *player, enum Direction direction,
                          const struct TerrainMap *terrain_map) {

    struct BoundingBox bb = player_make_bb(player);
    struct WorldCoordinate potential_loc = player->loc;
    player_make_potential_move(&bb, &potential_loc, direction);

    bool can_pass =
        terrain_is_check_all_target(&bb, terrain_map, player_is_point_walkable);

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

void handle_movement(struct Player *player,
                     const struct TerrainMap *terrain_map,
                     const struct InputState *inputs) {
    if (player->is_animating) {
        player->is_animating = animtion_next_frame(&player->animation);
    }
    enum Direction d;
    if (input_get_pressed_direction(inputs, INPUT_AXIS_VERTICAL, &d)) {
        move_player_if_valid(player, d, terrain_map);
    }
    if (input_get_pressed_direction(inputs, INPUT_AXIS_HORIZONTAL, &d)) {
        move_player_if_valid(player, d, terrain_map);
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
