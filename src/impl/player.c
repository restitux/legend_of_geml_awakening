#include "player.h"
#include "../../res/map/tiled.h"
#include "room_renderer.h"
#include "types.h"
#include "wasm4.h"
#include "sprite.h"

void move_player_if_valid(struct Player *player, enum Direction direction,
                          const struct TileMap_DataLayer *collision_map) {
    struct WorldCoordinate new_loc_tl = player->loc;
    struct WorldCoordinate new_loc_tr = player->loc;
    new_loc_tr.screen.x += 7;
    struct WorldCoordinate new_loc_bl = player->loc;
    new_loc_bl.screen.y += 7;
    struct WorldCoordinate new_loc_br = player->loc;
    new_loc_br.screen.y += 7;
    new_loc_br.screen.x += 7;
    struct WorldCoordinate collision_loc = player->loc;

    // increment position of all corners based on input direction
    switch (direction) {
    case DIRECTION_UP:
        new_loc_tl.screen.y -= 1;
        new_loc_tr.screen.y -= 1;
        new_loc_bl.screen.y -= 1;
        new_loc_br.screen.y -= 1;
        break;
    case DIRECTION_DOWN:
        new_loc_tl.screen.y += 1;
        new_loc_tr.screen.y += 1;
        new_loc_bl.screen.y += 1;
        new_loc_br.screen.y += 1;
        break;
    case DIRECTION_LEFT:
        new_loc_tl.screen.x -= 1;
        new_loc_tr.screen.x -= 1;
        new_loc_bl.screen.x -= 1;
        new_loc_br.screen.x -= 1;
        break;
    case DIRECTION_RIGHT:
        new_loc_tl.screen.x += 1;
        new_loc_tr.screen.x += 1;
        new_loc_bl.screen.x += 1;
        new_loc_br.screen.x += 1;
        break;
    default:
        break;
    }
    // collide all corners
    int tile = room_tile_at_screen_coordinates(&new_loc_tl, collision_map) ||
               room_tile_at_screen_coordinates(&new_loc_tr, collision_map) ||
               room_tile_at_screen_coordinates(&new_loc_bl, collision_map) ||
               room_tile_at_screen_coordinates(&new_loc_br, collision_map);
    // if no corners are colliding update position
    if (!tile) {
        player->loc = new_loc_tl;
    }
}

void handle_movement(struct Player *player,
                     const struct TileMap_DataLayer *collision_map) {
    uint8_t gamepad = *GAMEPAD1;

    if (gamepad & BUTTON_UP) {
        move_player_if_valid(player, DIRECTION_UP, collision_map);
    }
    if (gamepad & BUTTON_DOWN) {
        move_player_if_valid(player, DIRECTION_DOWN, collision_map);
    }
    if (gamepad & BUTTON_LEFT) {
        move_player_if_valid(player, DIRECTION_LEFT, collision_map);
    }
    if (gamepad & BUTTON_RIGHT) {
        move_player_if_valid(player, DIRECTION_RIGHT, collision_map);
    }
}

void draw_player(const struct Player *player) {
    // // blitSub(player->sprite, player->loc.screen.x, player->loc.screen.y, 16, 16, )
    // text("P", player->loc.screen.x, player->loc.screen.y);

    draw_sprite(&player->sprite, &player->loc.screen);
}
