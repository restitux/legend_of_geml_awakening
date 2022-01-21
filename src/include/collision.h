#pragma once

#include "../../res/map/tiled.h"
#include "room.h"
#include "terrain_map.h"
#include "types.h"

struct Block;

void terrain_map_update(struct TerrainMap *t, struct RoomBlocks *blocks,
                        struct RoomCoordinate loc, const struct TileMap *map);

uint8_t terrain_at_point(const struct TerrainMap *t, struct ScreenCoordinate p);

uint8_t terrain_type(Terrain terrain, enum TerrainLayer layer);

Terrain terrain_create(enum TerrianType t, enum TerrainLayer l);

void terrain_debug_draw(const struct TerrainMap *t);