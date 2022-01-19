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
  struct ScreenCoordinate tr;
  struct ScreenCoordinate bl;
  struct ScreenCoordinate br;
};

enum Direction {
  DIRECTION_UP = 0,
  DIRECTION_DOWN = 1,
  DIRECTION_LEFT = 2,
  DIRECTION_RIGHT = 3,
};

struct GridCoordinate
coordinate_screen_to_grid(const struct ScreenCoordinate *c);

struct ScreenCoordinate
coordinate_grid_to_screen(const struct GridCoordinate *c);

void coordinate_align_to_grid(struct ScreenCoordinate *c);

struct BoundingBox bounding_box_new(struct ScreenCoordinate top_left,
                                    uint32_t width, uint32_t height);

bool bounding_box_intersect(const struct BoundingBox *a,
                            const struct BoundingBox *b);

void debug_bb_draw(const struct BoundingBox *b);