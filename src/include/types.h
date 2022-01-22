#pragma once

#include <stdbool.h>
#include <stdint.h>

// represents an x/y screen location for grid aligned objects
struct GridCoordinate {
    uint8_t x;
    uint8_t y;
};

// represents an x/y screen location for non-grid aligned objects
struct ScreenCoordinate {
    uint32_t x;
    uint32_t y;
};

// represents an x/y room coordinate on the map
struct RoomCoordinate {
    uint32_t x;
    uint32_t y;
};

struct WorldCoordinate {
    struct RoomCoordinate room;
    struct ScreenCoordinate screen;
};

struct BoundingBox {
    struct ScreenCoordinate tl;
    uint8_t width;
    uint8_t height;
};

enum Direction {
    DIRECTION_UP = 0,
    DIRECTION_DOWN = 1,
    DIRECTION_LEFT = 2,
    DIRECTION_RIGHT = 3,
};

enum CollisionType { COLLISION_TYPE_ALL = 0, COLLISION_TYPE_ANY = 1 };

struct GridCoordinate
coordinate_screen_to_grid(const struct ScreenCoordinate *c);

struct ScreenCoordinate
coordinate_grid_to_screen(const struct GridCoordinate *c);

struct WorldCoordinate coordinate_global_to_world(uint32_t x, uint32_t y);

void coordinate_align_to_grid(struct ScreenCoordinate *c);

struct ScreenCoordinate coordinate_screen_add(struct ScreenCoordinate a,
                                              struct ScreenCoordinate b);

struct ScreenCoordinate
coordinate_screen_add_direction(struct ScreenCoordinate a, enum Direction dir,
                                uint8_t amount);

struct BoundingBox bounding_box_new(struct ScreenCoordinate top_left,
                                    uint32_t width, uint32_t height);

bool bounding_box_intersect(const struct BoundingBox *a,
                            const struct BoundingBox *b);

void bounding_box_corners(const struct BoundingBox *b,
                          struct ScreenCoordinate out[4]);

void bounding_box_uniform_shrink(struct BoundingBox *b, uint8_t amount);

void debug_bb_draw(const struct BoundingBox *b);

bool bounding_box_contains_point(const struct BoundingBox *b,
                                 struct ScreenCoordinate p);

enum Direction direction_reverse(enum Direction d);