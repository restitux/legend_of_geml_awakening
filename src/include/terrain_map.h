#include "../../res/map/tiled.h"
#pragma once

typedef uint8_t Terrain;

enum TerrianType {
    TERRAIN_NORMAL = 0,
    TERRAIN_WALL = 1,
    TERRAIN_SLIPPERY = 2,
    TERRAIN_BLOCK = 3,
    TERRAIN_LAVA = 4,

    TERRAIN_INVALID = 0x0F,
};

enum TerrainLayer {
    LAYER_MAIN = 0,
    LAYER_LOWER = 1,
};

struct TerrainMap {
    Terrain terrain[400];
    const struct TileMap *base_map;
    struct RoomCoordinate room_loc;
};