#include "sprite.h"
#include "wasm4.h"

void sprite_draw_sprite_frame(const struct SpriteFrame *sprite,
                              const struct ScreenCoordinate *loc) {
  uint16_t old_colors = *DRAW_COLORS;
  *DRAW_COLORS = 0x3204;
  blitSub(sprite->sprite_sheet, loc->x, loc->y, 16, 16, sprite->x, sprite->y,
          sprite->sheet_width, BLIT_2BPP);
  *DRAW_COLORS = old_colors;
}

void sprite_advance_animation(struct CharacterSprite *sprite) {
  sprite->current_frame += 1;
  sprite->current_frame %= (sprite->sheet_height / sprite->frame_size) *
                           sprite->frames_per_animation;
}

void sprite_draw_character(const struct CharacterSprite *sprite,
                           const struct ScreenCoordinate *loc,
                           enum Direction dir) {
  uint32_t x = dir * sprite->frame_size;
  uint32_t y = (sprite->current_frame / sprite->frames_per_animation) *
               sprite->frame_size;
  uint16_t old_colors = *DRAW_COLORS;
  *DRAW_COLORS = 0x4320;
  blitSub(sprite->sprite_sheet, loc->x, loc->y, sprite->frame_size,
          sprite->frame_size, x, y, sprite->sheet_width, BLIT_2BPP);
  *DRAW_COLORS = old_colors;
}