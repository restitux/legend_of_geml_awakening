#include "player.h"
#include "room_renderer.h"
#include "wasm4.h"

void handle_movement(struct Player *player, struct Datamap *collision_map) {
    uint8_t gamepad = *GAMEPAD1;

    if (gamepad & BUTTON_UP) {
        struct WorldCoordinate new_loc = player->loc;
        new_loc.screen.y -= 1;
        int tile = room_tile_at_screen_coordinates(&new_loc, collision_map);
        if (!tile) {
            player->loc = new_loc;
        }
    }
    if (gamepad & BUTTON_DOWN) {
        struct WorldCoordinate new_loc = player->loc;
        new_loc.screen.y += 1;
        int tile = room_tile_at_screen_coordinates(&new_loc, collision_map);
        if (!tile) {
            player->loc = new_loc;
        }
    }
    if (gamepad & BUTTON_LEFT) {
        struct WorldCoordinate new_loc = player->loc;
        new_loc.screen.x -= 1;
        int tile = room_tile_at_screen_coordinates(&new_loc, collision_map);
        if (!tile) {
            player->loc = new_loc;
        }
    }
    if (gamepad & BUTTON_RIGHT) {
        struct WorldCoordinate new_loc = player->loc;
        new_loc.screen.x += 1;
        int tile = room_tile_at_screen_coordinates(&new_loc, collision_map);
        if (!tile) {
            player->loc = new_loc;
        }
    }
}

void draw_player(const struct Player *player) {
    text("P", player->loc.screen.x, player->loc.screen.y);
}
