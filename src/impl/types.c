#include "types.h"

struct GridCoordinate
coordinate_screen_to_grid(const struct ScreenCoordinate *c) {
  return (struct GridCoordinate){
      .x = c->x / 8,
      .y = c->y / 8,
  };
}
struct ScreenCoordinate
coordinate_grid_to_screen(const struct GridCoordinate *c) {
  return (struct ScreenCoordinate){
      .x = c->x * 8,
      .y = c->y * 8,
  };
}

void coordinate_align_to_grid(struct ScreenCoordinate *c) {
  c->x = (c->x / 8) * 8;
  c->y = (c->y / 8) * 8;
}