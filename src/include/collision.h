#pragma once

#include "../../res/map/tiled.h"
#include "room_renderer.h"
#include "types.h"

struct RoomBlocks;

typedef uint8_t Terrain;

enum TerrianType {
    TERRAIN_NORMAL = 0,
    TERRAIN_WALL = 1,
    TERRAIN_SLIPPERY = 2,
    TERRAIN_BLOCK = 4,
};

enum TerrainLayer {
    LAYER_MAIN = 0,
    LAYER_LOWER = 1,
};

struct TerrainMap {
    Terrain terrain[400];
};

void terrain_map_update(struct TerrainMap *t, struct RoomBlocks *blocks,
                        struct RoomCoordinate loc, const struct TileMap *map);

uint8_t terrain_at_point(const struct TerrainMap *t, struct ScreenCoordinate p);

uint8_t terrain_type(Terrain terrain);

uint8_t terrain_layer(Terrain terrain);

void terrain_debug_draw(const struct TerrainMap *t);