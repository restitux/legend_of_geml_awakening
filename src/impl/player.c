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

void move_player_if_valid(struct Player *player, enum Direction direction,
                          const struct TerrainMap *terrain_map) {

    struct BoundingBox bb = player_make_bb(player);
    struct WorldCoordinate potential_loc = player->loc;
    switch (direction) {
    case DIRECTION_UP:
        bb.tl.y -= 1;
        potential_loc.screen.y -= 1;
        break;

    case DIRECTION_DOWN:
        bb.tl.y += 1;
        potential_loc.screen.y += 1;
        break;

    case DIRECTION_LEFT:
        bb.tl.x -= 1;
        potential_loc.screen.x -= 1;
        break;

    case DIRECTION_RIGHT:
        bb.tl.x += 1;
        potential_loc.screen.x += 1;
        break;
    default:
        break;
    }

    struct ScreenCoordinate corners[4];
    bounding_box_corners(&bb, corners);

    bool can_pass = true;
    for (int i = 0; i < 4; i++) {
        Terrain t = terrain_at_point(terrain_map, corners[i]);

        // enum TerrianType cur_layer = terrain_type(t, player->layer);
        // // enum TerrainLayer layer = terrain_type(t);
        enum TerrianType cur_layer = terrain_type(t, player->layer);
        enum TerrianType lower_layer = terrain_type(t, player->layer + 1);

        if (cur_layer == TERRAIN_WALL) {
            can_pass = false;
        }
        if (cur_layer == TERRAIN_INVALID && lower_layer != TERRAIN_BLOCK &&
            lower_layer != TERRAIN_SLIPPERY) {
            can_pass = false;
        }
    }

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
