#pragma once

#include "stdint.h"
#include "types.h"


struct Sprite {
    const uint8_t *sprite_sheet;
    uint8_t index;
};

void draw_sprite(struct Sprite *sprite, struct ScreenCoordinate *loc);