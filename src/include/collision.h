#pragma once

#include "../../res/map/tiled.h"
#include "room.h"
#include "terrain_map.h"
#include "types.h"

struct Block;

typedef bool (*TerrainPointCheck)(Terrain t);

void terrain_map_update(struct TerrainMap *t, struct Room *room,
                        const struct TileMap *map);

uint8_t terrain_at_point(const struct TerrainMap *t, struct ScreenCoordinate p);

uint8_t terrain_type(Terrain terrain, enum TerrainLayer layer);

Terrain terrain_create(enum TerrianType t, enum TerrainLayer l);

void terrain_debug_draw(const struct TerrainMap *t);

bool terrain_is_slide_target(const struct BoundingBox *target_box,
                             const struct TerrainMap *tm);

uint8_t terrain_calc_slide_distance(struct BoundingBox moved, enum Direction d,
                                    const struct TerrainMap *tm,
                                    TerrainPointCheck slide_over_check);

bool terrain_is_check_all_target(const struct BoundingBox *target_box,
                                 const struct TerrainMap *tm,
                                 TerrainPointCheck check);

bool terrain_is_point_slidable(Terrain t);