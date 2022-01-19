#include "room_renderer.h"
#include <stdbool.h>
#include <stdint.h>

#include "../../res/map/tiled.h"
#include "wasm4.h"

void room_draw_tile(uint16_t tile, uint32_t x, uint32_t y,
                    const uint8_t *tileset) {
    uint16_t tile_id = (tile & 0xFF);
    // uint32_t rotations = ((tile & (0xF << 28)) >> 28);
    // uint32_t rotations = ((tile & (0xF << 12)) >> 12);
    uint16_t rotations = ((tile & (0xF << ((sizeof(tile) * 8) - 4))) >>
                          ((sizeof(tile) * 8) - 4));
    //  uint32_t rotations = ((tile & (0xF << ((sizeof(tile) * 8) - 4))) >>
    //                      ((sizeof(tile) * 8) - 4));

    uint32_t flags = BLIT_2BPP;

    bool flipx = rotations & 0b1000;
    bool flipy = rotations & 0b0100;
    bool rotate = rotations & 0b0010;

    if (flipx) {
        flags |= BLIT_FLIP_X;
    }
    if (flipy) {
        flags |= BLIT_FLIP_Y;
    }
    if (rotate) {
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
                    const struct TileMap_MapLayer *map) {
    int room_start_y = room_y * 20;
    int room_start_x = room_x * 20;
    for (int x = room_start_x; x < room_start_x + 20; x++) {
        for (int y = room_start_y; y < room_start_y + 20; y++) {
            int tile_index = x + (y * map->width);
            room_draw_tile(map->map[tile_index], x - room_start_x,
                           y - room_start_y, map->tileset->tileset);
        }
    }
}

int room_tile_at_screen_coordinates(struct WorldCoordinate *loc,
                                    const struct TileMap_DataLayer *map) {
    int world_x = (loc->screen.x / 8) + (loc->room.x * 20);
    int world_y = (loc->screen.y / 8) + (loc->room.y * 20);

    int index = world_x + (world_y * map->width);
    return map->map[index];
}

int room_is_tile_present_at_bb_corners(const struct BoundingBox *bb,
                                       const struct TileMap_DataLayer *map,
                                       struct RoomCoordinate room) {
    struct ScreenCoordinate bb_corners[4];
    bounding_box_corners(bb, bb_corners);
    int tile = 0;
    for (int i = 0; i < 4; i++) {
        struct WorldCoordinate w = (struct WorldCoordinate){
            .screen = bb_corners[i],
            .room = room,
        };
        tile |= room_tile_at_screen_coordinates(&w, map);
    }
    return tile;
}