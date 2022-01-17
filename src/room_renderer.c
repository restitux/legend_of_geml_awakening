#include "room_renderer.h"
#include <stdbool.h>
#include <stdint.h>

#include "tilemap.h"
#include "wasm4.h"

void room_draw_tile(uint32_t tile, uint32_t x, uint32_t y,
                    const uint8_t *tileset) {
    uint32_t tile_id = (tile & 0xFF);
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
        if (flipy) {
            flags &= ~BLIT_FLIP_Y;
        } else {
            flags |= BLIT_FLIP_Y;
        }
    }

    int byte = tile_id * 16;

    blit(tileset + byte, x * 8, y * 8, 8, 8, flags);
}

void room_draw_room(uint32_t room_x, uint32_t room_y,
                    const struct Tilemap *map) {
    int room_start_y = room_y * 20;
    int room_start_x = room_x * 20;
    for (int x = room_start_x; x < room_start_x + 20; x++) {
        for (int y = room_start_y; y < room_start_y + 20; y++) {
            int tile_index = x + (y * map->map_width);
            room_draw_tile(map->map[tile_index], x - room_start_x,
                           y - room_start_y, map->tileset);
        }
    }
}

int room_tile_at_screen_coordinates(struct WorldCoordinate *loc,
                                    const struct Datamap *map) {
    int world_x = (loc->screen.x / 8) + loc->room.x;
    int world_y = (loc->screen.y / 8) + loc->room.y;

    int index = world_x + (world_y * map->map_width);
    return map->map[index];
}
