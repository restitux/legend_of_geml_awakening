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
  bb.tr = top_left;
  bb.bl = top_left;
  bb.br = top_left;

  bb.tr.x += width;

  bb.bl.y -= height;

  bb.br.x += width;
  bb.br.y -= height;

  return bb;
}

bool bounding_box_intersect(const struct BoundingBox *a,
                            const struct BoundingBox *b) {
  bool x_overlap = (a->tr.x > b->tl.x) && (b->tr.x > a->tl.x);
  bool y_overlap = (a->bl.y > b->tl.y) && (b->tr.y > a->tl.y);

  return x_overlap && y_overlap;
}

void debug_bb_draw(const struct BoundingBox *b) {
  uint16_t old_colors = *DRAW_COLORS;
  *DRAW_COLORS = 0x0030;
  rect(b->tl.x, b->tl.y, 16, 16);
  *DRAW_COLORS = old_colors;
}