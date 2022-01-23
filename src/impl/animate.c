#include "animate.h"
#include "types.h"
#include <stdint.h>

bool animtion_next_frame(struct Animation *a) {
    if (a->frames_remaining == 0) {
        *(a->animation_subject) = a->end_loc;
        return false;
    }
    if ((a->frames_remaining % a->frame_per_move) == 0) {
        *(a->animation_subject) = coordinate_screen_add_direction(
            *(a->animation_subject), a->move_direction, 1);
    }
    a->frames_remaining -= 1;

        return true;
}