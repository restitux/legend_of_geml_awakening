#include <stdbool.h>

#include "wasm4.h"

#include "tile_renderer.h"

#include "../res/map/testing.map.h"
#include "../res/map/new_tiles.set.h"

const unsigned int SCREEN_W = 160;
const unsigned int SCREEN_H = 160;


// const uint8_t tilemap[] = TILESET_tiles;
// const uint32_t tilesize = TILESIZE_tiles;


struct PlayerData {
    unsigned int x;
    unsigned int y;
};

struct GameState {
    struct PlayerData player_data;
    const uint32_t *tilemap;
    const uint8_t *tileset;
} state = {
    .player_data = {
        .x = 0,
        .y = 0,
    },
    .tilemap = testing_tilemap,
    .tileset = new_tiles_tileset,
};

//void render_player() {
//    blit(PLAYER_SPRITE, player_data.x, SCREEN_H - player_data.y - 32, 16, 32, BLIT_2BPP);
//}
//
//bool debounce = false;


// int tile_id(int x, int y) {
//     return x + y * 16;
// }

// int tile_byte(int id) {
//     return id * 16;
// }

// void draw_tile(int x, int y) {
//     int id = tile_id(x, y);
//     int byte = tile_byte(id);
//     blit(tilemap + byte, x*8, y*8, 8, 8, BLIT_2BPP);
// }



void update () {
    PALETTE[0] = 0x00FF0000;
    PALETTE[1] = 0x00000000;
    PALETTE[2] = 0x00444444;
    PALETTE[3] = 0x00FFFFFF;
    *DRAW_COLORS = 0x4321;
    //text("Hello from C!", 10, 10);

    uint8_t gamepad = *GAMEPAD1;
    if (gamepad & BUTTON_1) {
        state.player_data.x += 2;
    }

    draw_map(state.tilemap, state.tileset);

    // draw_tile(1, 0, 0, state.tileset);

    // int i = 0;
    // for (int y = 0; y < 16; y++) {
    //     for (int x = 0; x < 16; x++) {
    //         draw_tile(i, x, y);
    //         i++;
    //     }
    // }
    
}
