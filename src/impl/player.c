#include "player.h"
#include "../../res/map/tiled.h"
#include "configuration.h"
#include "hooks.h"
#include "room_renderer.h"
#include "sprite.h"
#include "types.h"
#include "wasm4.h"

void move_player_if_valid(struct Player *player, enum Direction direction,
                          const struct TileMap *map, const struct Room *room) {
    struct BoundingBox bb = bounding_box_new(player->loc.screen, 16, 16);
    bounding_box_uniform_shrink(&bb, PLAYER_COLLISION_BUFFER);

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
    int tile = room_is_tile_present_at_bb_corners(
        &bb, &map->collision_map, potential_loc.room, COLLISION_TYPE_ANY);
    int special_tile = room_is_tile_present_at_bb_corners(
        &bb, &map->special_map, potential_loc.room, COLLISION_TYPE_ANY);

    if (special_tile) {
        for (uint32_t i = 0; i < room->blocks.size; i++) {
            struct Block *b = &room->blocks.b[i];
            struct BoundingBox block_bb =
                bounding_box_new(b->draw_loc, BLOCK_SIZE, BLOCK_SIZE);
            bounding_box_uniform_shrink(&bb, -PLAYER_COLLISION_BUFFER);
            if (bounding_box_intersect(&bb, &block_bb)) {
                special_tile = 0;
            }
        }
    }

    if (!tile && !special_tile) {

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

void handle_movement(struct Player *player, const struct TileMap *collision_map,
                     const struct InputState *inputs, const struct Room *room) {

    enum Direction d;
    if (input_get_pressed_direction(inputs, INPUT_AXIS_VERTICAL, &d)) {
        move_player_if_valid(player, d, collision_map, room);
    }
    if (input_get_pressed_direction(inputs, INPUT_AXIS_HORIZONTAL, &d)) {
        move_player_if_valid(player, d, collision_map, room);
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
