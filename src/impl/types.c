#include "types.h"

#include <wasm4.h>

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
      .x = (c->x - 1) * 8,
      .y = (c->y - 1) * 8,
  };
}

void coordinate_align_to_grid(struct ScreenCoordinate *c) {
  c->x = (c->x / 8) * 8;
  c->y = (c->y / 8) * 8;
}

struct BoundingBox bounding_box_new(struct ScreenCoordinate top_left,
                                    uint32_t width, uint32_t height) {
  struct BoundingBox bb;

  bb.tl = top_left;
  bb.width = width;
  bb.height = height;

  return bb;
}

bool bounding_box_axis_overlap(uint8_t a_max_x, uint8_t a_min_x,
                               uint8_t b_max_x, uint8_t b_min_x) {
  return (a_max_x > b_min_x) && (b_max_x > a_min_x);
}

bool bounding_box_intersect(const struct BoundingBox *a,
                            const struct BoundingBox *b) {

  uint8_t a_min_x = a->tl.x;
  uint8_t a_max_x = a->tl.x + a->width;

  uint8_t a_min_y = a->tl.y;
  uint8_t a_max_y = a->tl.y + a->height;

  uint8_t b_min_x = b->tl.x;
  uint8_t b_max_x = b->tl.x + b->width;

  uint8_t b_min_y = b->tl.y;
  uint8_t b_max_y = b->tl.y + b->height;

  bool x_overlap =
      bounding_box_axis_overlap(a_max_x, a_min_x, b_max_x, b_min_x);
  bool y_overlap =
      bounding_box_axis_overlap(a_max_y, a_min_y, b_max_y, b_min_y);

  return x_overlap && y_overlap;
}

void debug_bb_draw(const struct BoundingBox *b) {
  uint16_t old_colors = *DRAW_COLORS;
  *DRAW_COLORS = 0x0030;
  rect(b->tl.x - 1, b->tl.y - 1, b->width + 2, b->height + 2);
  *DRAW_COLORS = old_colors;
}