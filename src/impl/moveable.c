#include "collision.h"
#include "configuration.h"
#include "movable.h"
#include "terrain_map.h"
#include "types.h"

struct MovableObject movable_object_new(struct ScreenCoordinate tl,
                                        uint8_t width, uint8_t height) {
    return (struct MovableObject){
        .bb =
            {
                .tl = tl,
                .width = width,
                .height = height,
            },
        .layer = LAYER_MAIN,
        .is_sliding = 0,
    };
}

bool is_slide_valid(struct MovableObject *obj,
                    const struct TerrainMap *terrain_map) {
    struct BoundingBox moved = obj->bb;
    moved.tl = coordinate_screen_add_direction(moved.tl, obj->last_moved_dir,
                                               GRID_SIZE * 2);

    struct ScreenCoordinate corners[4];
    bounding_box_corners(&moved, corners);
    debug_bb_draw(&moved);
    for (int i = 0; i < 4; i++) {
        if (bounding_box_contains_point(&obj->bb, corners[i])) {
            continue;
        }
        Terrain t = terrain_at_point(terrain_map, corners[i]);
        enum TerrianType cur_layer = terrain_type(t, obj->layer);
        if (cur_layer == TERRAIN_WALL || cur_layer == TERRAIN_BLOCK) {
            return false;
        }
    }

    return true;
}

uint8_t compute_slide_end(const struct MovableObject *obj,
                          const struct TerrainMap *tm) {
    enum Direction dir = obj->last_moved_dir;
    struct BoundingBox moved = obj->bb;
    moved.width - 1;
    moved.height - 1;
    moved.tl = coordinate_screen_add_direction(moved.tl, dir, GRID_SIZE);
    uint8_t move_dist = 0;
    while (true) {
        struct ScreenCoordinate corners[4];
        bounding_box_corners(&moved, corners);

        int ice_count = 0;
        for (int i = 0; i < 4; i++) {
            if (bounding_box_contains_point(&obj->bb, corners[i])) {
                ice_count += 1;
                tracef("inside self, ignoring");
                continue;
            }
            Terrain t = terrain_at_point(tm, corners[i]);
            enum TerrianType main_layer = terrain_type(t, LAYER_MAIN);
            enum TerrianType lower_layer = terrain_type(t, LAYER_LOWER);
            if (main_layer == TERRAIN_INVALID &&
                lower_layer == TERRAIN_SLIPPERY) {
                ice_count += 1;
            }
        }

        if (ice_count < 4) {
            break;
        } else {
            moved.tl =
                coordinate_screen_add_direction(moved.tl, dir, GRID_SIZE);
            move_dist += GRID_SIZE;
        }
    }
    if (move_dist > 0) {
        moved.tl = coordinate_screen_add_direction(moved.tl, dir, GRID_SIZE);
        struct ScreenCoordinate corners[4];
        bounding_box_corners(&moved, corners);
        int normal_count = 0;
        for (int i = 0; i < 4; i++) {
            Terrain t = terrain_at_point(tm, corners[i]);
            enum TerrianType cur_layer = terrain_type(t, LAYER_MAIN);
            if (cur_layer == TERRAIN_NORMAL) {
                normal_count += 1;
            } else {
                struct GridCoordinate g =
                    coordinate_screen_to_grid(&corners[i]);
                tracef("object ending slide because %d terrain at %d, %d",
                       cur_layer, g.x, g.y);
            }
        }
        if (normal_count == 4) {
            move_dist += BLOCK_SIZE;
        }
    }

    return move_dist;
}

bool should_start_slide(struct MovableObject *obj,
                        const struct TerrainMap *terrain_map) {

    if (!is_slide_valid(obj, terrain_map)) {
        return false;
    }

    struct ScreenCoordinate corners[4];
    struct BoundingBox moved = obj->bb;
    moved.tl = coordinate_screen_add_direction(moved.tl, obj->last_moved_dir,
                                               GRID_SIZE);
    bounding_box_corners(&moved, corners);
    int slippery_count = 0;
    for (int i = 0; i < 4; i++) {
        struct GridCoordinate grid = coordinate_screen_to_grid(&corners[i]);
        Terrain t = terrain_at_point(terrain_map, corners[i]);
        enum TerrianType cur_layer = terrain_type(t, LAYER_LOWER);
        if (cur_layer == TERRAIN_SLIPPERY) {
            slippery_count += 1;
        }
    }

    coordinate_align_to_grid(&obj->bb.tl);

    return slippery_count >= 1;
}

void movable_check_slide(struct MovableObject *obj,
                         const struct TerrainMap *terrain_map) {

    if (!obj->is_sliding && should_start_slide(obj, terrain_map)) {
        // trace("object is not sliding");
        obj->is_sliding = compute_slide_end(obj, terrain_map);
    }
}

void movable_update_slide(struct MovableObject *obj,
                          const struct TerrainMap *terrain_map) {
    if (obj->is_sliding) {
        obj->bb.tl =
            coordinate_screen_add_direction(obj->bb.tl, obj->last_moved_dir, 1);
        obj->is_sliding -= 1;

        if (terrain_map) {
        }
    }
}