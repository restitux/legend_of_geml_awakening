#pragma once

#include "stdbool.h"
#include "stdint.h"
#include "types.h"

// a single sprite with no animation
struct SpriteFrame {
  const uint8_t *sprite_sheet;
  uint8_t x;
  uint8_t y;
};

// character sprite is encoded with a sheet of 4 frames x n animation frames
struct CharacterSprite {
  const uint8_t *sprite_sheet;
  uint32_t sheet_width;
  uint32_t sheet_height;
  uint32_t frame_size;
  uint32_t frames_per_animation;

  uint32_t current_frame;
};

void sprite_draw_sprite_frame(const struct SpriteFrame *sprite,
                              const struct ScreenCoordinate *loc);

void sprite_advance_animation(struct CharacterSprite *sprite);

void sprite_draw_character(const struct CharacterSprite *sprite,
                           const struct ScreenCoordinate *loc,
                           enum Direction dir);