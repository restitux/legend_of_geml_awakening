#pragma once

#include "types.h"
#include <stdint.h>

struct Animation {
    struct ScreenCoordinate *animation_subject;
    struct ScreenCoordinate end_loc;

    uint8_t frames_remaining;
    enum Direction move_direction;
    uint8_t frame_per_move;
};

// return false if animation is over after this frame
bool animtion_next_frame(struct Animation *a);