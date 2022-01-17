#include <stdbool.h>

#include "wasm4.h"

#include "tile_renderer.h"

#include "../../res/map/testing.map.h"
#include "../../res/map/tiled.h"
#include "../../res/map/tiles.set.h"
#include "../../res/data/player_animation.h"

#include "player.h"
#include "room_renderer.h"

const unsigned int SCREEN_W = 160;
const unsigned int SCREEN_H = 160;

struct PlayerData {
    unsigned int x;
    unsigned int y;
};

struct Player player = {
    .loc = {.room = {0, 0}, .screen = {35, 70}}, 
    .sprite = {
        .sprite_sheet=player_animation,
        .sheet_height=player_animationHeight,
        .sheet_width=player_animationWidth,
        .frame_size=player_animationFrameSize,
        .current_frame=0,
        .frames_per_animation=10
    },
    .player_height = 16,
    .player_width = 16      
};
const struct TileMap *map = &testing_tilemap;

void update() {
    PALETTE[0] = 0x00FF0000;
    PALETTE[1] = 0x00000000;
    PALETTE[2] = 0x00444444;
    PALETTE[3] = 0x00FFFFFF;
    *DRAW_COLORS = 0x4321;

    handle_movement(&player, &map->collision_map);
    room_draw_room(player.loc.room.x, player.loc.room.y, &map->static_map);

    draw_player(&player);

    if (player.loc.screen.x > 159) {
        player.loc.screen.x = 1;
        player.loc.room.x += 1;
    }
    if (player.loc.screen.x < 1) {
        player.loc.screen.x = 159;
        player.loc.room.x -= 1;
    }
    if (player.loc.screen.y > 159) {
        player.loc.screen.y = 1;
        player.loc.room.y += 1;
    }
    if (player.loc.screen.y < 1) {
        player.loc.screen.y = 159;
        player.loc.room.y -= 1;
    }
}
