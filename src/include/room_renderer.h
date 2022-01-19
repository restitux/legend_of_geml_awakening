#pragma once

#include "../../res/map/tiled.h"
#include "block.h"
#include "types.h"
#include <stdint.h>

struct RoomBlocks {
    struct Block *b;
    uint32_t size;
};

struct Room {
    struct RoomCoordinate loc;

    struct RoomBlocks blocks;
};

void room_draw_room(uint32_t room_x, uint32_t room_y,
                    const struct TileMap_MapLayer *map);

bool room_tile_at_screen_coordinates(struct WorldCoordinate *loc,
                                     const struct TileMap_DataLayer *map);

int room_is_tile_present_at_bb_corners(const struct BoundingBox *bb,
                                       const struct TileMap_DataLayer *map,
                                       struct RoomCoordinate room,
                                       enum CollisionType type);

void room_draw_room_debug_map(uint32_t room_x, uint32_t room_y,
                              const struct TileMap_DataLayer *map);

void room_draw_room_special_tiles(uint32_t room_x, uint32_t room_y,
                                  const struct TileMap_DataLayer *map);