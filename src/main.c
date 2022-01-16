#include <stdbool.h>

#include "wasm4.h"

#include "tile_renderer.h"

const uint8_t smiley[] = {
    0b11000011,
    0b10000001,
    0b00100100,
    0b00100100,
    0b00000000,
    0b00100100,
    0b10011001,
    0b11000011,
};

const uint8_t PLAYER_SPRITE[] = {
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b01010100, 0b00000000,
    0b00000000, 0b00000001, 0b10101001, 0b01010000,
    0b00000000, 0b00000110, 0b10101010, 0b10100100,
    0b00000000, 0b00011010, 0b10101010, 0b01010100,
    0b00000000, 0b01101010, 0b10101001, 0b00000100,
    0b00000001, 0b10101010, 0b10101001, 0b00000000,
    0b00010110, 0b10101010, 0b10101010, 0b01000000,
    0b01011010, 0b10101010, 0b10101010, 0b01000000,
    0b00010101, 0b10101010, 0b10101010, 0b10010100,
    0b00000001, 0b10010101, 0b01011010, 0b10101001,
    0b00000000, 0b01100111, 0b10110101, 0b01010100,
    0b00000000, 0b01100111, 0b10110100, 0b00000000,
    0b00000001, 0b10101010, 0b10110100, 0b00000000,
    0b00000000, 0b01111111, 0b11110101, 0b01000000,
    0b00000000, 0b01111111, 0b01011010, 0b10010000,
    0b00000000, 0b01111111, 0b01100110, 0b10010000,
    0b00000000, 0b01111111, 0b01100110, 0b10010000,
    0b00000001, 0b01011101, 0b10100110, 0b10010000,
    0b00000111, 0b01011101, 0b10011111, 0b11010000,
    0b00000111, 0b01011110, 0b10011111, 0b11010000,
    0b00000101, 0b01100110, 0b10011111, 0b11010000,
    0b00000001, 0b01100110, 0b10100101, 0b01010000,
    0b00000000, 0b01100110, 0b10101010, 0b10010000,
    0b00000001, 0b10100110, 0b10101010, 0b10100100,
    0b00000001, 0b10100110, 0b10101001, 0b10100100,
    0b00000110, 0b10100110, 0b10101001, 0b10100100,
    0b00000101, 0b01010101, 0b01010101, 0b01010100,
};

const unsigned int SCREEN_W = 160;
const unsigned int SCREEN_H = 160;


// const uint8_t tilemap[] = TILESET_tiles;
// const uint32_t tilesize = TILESIZE_tiles;


struct PlayerData {
    unsigned int x;
    unsigned int y;
} player_data = {
    .x = 0,
    .y = 5
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
    PALETTE[0] = 0x00DDDDD0;
    PALETTE[1] = 0x00000000;
    PALETTE[2] = 0x00444444;
    PALETTE[3] = 0x00888888;
    *DRAW_COLORS = 0x4321;
    //text("Hello from C!", 10, 10);

    uint8_t gamepad = *GAMEPAD1;
    if (gamepad & BUTTON_1) {
        player_data.x += 2;
    }

    draw_map();

    // int i = 0;
    // for (int y = 0; y < 16; y++) {
    //     for (int x = 0; x < 16; x++) {
    //         draw_tile(i, x, y);
    //         i++;
    //     }
    // }
    
}
