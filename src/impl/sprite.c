#include "sprite.h"
#include "wasm4.h"

void draw_sprite(const struct Sprite *sprite, const struct ScreenCoordinate *loc) {
    blitSub(sprite->sprite_sheet, loc->x, loc->y, 16, 16, sprite->x, sprite->y, 128, BLIT_2BPP);    
}