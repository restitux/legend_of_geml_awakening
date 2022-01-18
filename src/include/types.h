#pragma once

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