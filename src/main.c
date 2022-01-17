#include <stdbool.h>

#include "wasm4.h"

#include "tile_renderer.h"

#include "../res/map/testing.map.h"
#include "../res/map/tiles.set.h"

#include "player.h"
#include "room_renderer.h"
#include "tilemap.h"

const unsigned int SCREEN_W = 160;
const unsigned int SCREEN_H = 160;

// const uint8_t tilemap[] = TILESET_tiles;
// const uint32_t tilesize = TILESIZE_tiles;

struct PlayerData {
    unsigned int x;
    unsigned int y;
};

struct Player player = {.loc = {.room = {0, 0}, .screen = {35, 70}}};
struct Tilemap map = {testing_static_tilemap, testing_static_WIDTH,
                      testing_static_HEIGHT, tiles_tileset};
struct Datamap collision_map = {testing_collision_tilemap,
                                testing_collision_WIDTH, testing_static_HEIGHT};

void update() {
    PALETTE[0] = 0x00FF0000;
    PALETTE[1] = 0x00000000;
    PALETTE[2] = 0x00444444;
    PALETTE[3] = 0x00FFFFFF;
    *DRAW_COLORS = 0x4321;

    handle_movement(&player, &collision_map);
    room_draw_room(player.loc.room.x, player.loc.room.y, &map);

    *DRAW_COLORS = 0x4901;
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
