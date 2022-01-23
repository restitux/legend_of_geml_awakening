#ifndef __TEXT_H_
#define __TEXT_H_

#include "../../res/map/tiled.h"
#include "player.h"

#include <stdbool.h>
#include <stdint.h>

struct TextState {
    bool displaying;
    uint16_t progress;
    uint16_t render_speed;
    uint32_t counter;
    uint16_t remain_on_screen_time;
    struct TileMap_TextTrigger current_text;
};

void handle_text_triggers(struct TextState *state,
                          struct TileMap_TextTriggers triggers,
                          struct Player player);
void render_text(struct TextState *state);

#endif // __TEXT_H_
