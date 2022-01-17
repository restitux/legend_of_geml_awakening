#pragma once

#include "stdint.h"
#include "types.h"


struct Sprite {
    const uint8_t *sprite_sheet;
    uint8_t x;
    uint8_t y;
};

void draw_sprite(const struct Sprite *sprite, const struct ScreenCoordinate *loc);