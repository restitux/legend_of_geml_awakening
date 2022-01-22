#pragma once

#include "terrain_map.h"
#include "types.h"

struct MovableObject {
    struct BoundingBox bb;

    enum TerrainLayer layer;

    uint8_t is_sliding;
    enum Direction last_moved_dir;
};

struct MovableObject movable_object_new(struct ScreenCoordinate tl,
                                        uint8_t width, uint8_t height);

void movable_check_slide(struct MovableObject *obj,
                         const struct TerrainMap *terrain_map);

void movable_update_slide(struct MovableObject *obj,
                          const struct TerrainMap *terrain_map);