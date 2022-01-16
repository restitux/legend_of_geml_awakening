#include "wasm4.h"
#include <stdbool.h>


int tile_byte(int id) {
    return id * 16;
}

void draw_tile(uint8_t id, uint32_t x, uint32_t y, const uint8_t *tileset) {
    int byte = tile_byte(id);

    blit(tileset + tile_byte(id), x * 8, y * 8, 8, 8, BLIT_2BPP);
}

void draw_map(const uint32_t *tilemap, const uint8_t *tileset) {
    for (int i = 0; i < 400; i++) {
        uint32_t tile = tilemap[i];

        uint32_t tile_id = (tile & 0xFF) - 1;
        uint32_t rotations = ((tile & (0xF << 28)) >> 28);
        
        uint32_t flags = BLIT_2BPP;

        bool flipx = rotations & 0b1000;
        bool flipy = rotations & 0b0100;

        if (flipx) {
            flags |= BLIT_FLIP_X;
        }
        if (flipy) {
            flags |= BLIT_FLIP_Y;
        }
        if (rotations & 0b0010) {
            flags |= BLIT_ROTATE;

            if (flipx) {
                // flags &= ~BLIT_FLIP_X; 
            } else {
                // flags |= BLIT_FLIP_X;
            }
            if (flipy) {
                flags &= ~BLIT_FLIP_Y;
            } else {
                flags |= BLIT_FLIP_Y;
            }
        }

        int x = i % 20;
        int y = i / 20;

        int byte = tile_byte(tile_id);

        //blit(tilemap + byte, x * 8, y * 8, 8, 8, flags);
        blit(tileset + byte, x * 8, y * 8, 8, 8, flags);
    }
}
