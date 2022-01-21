#pragma once

#include <stdbool.h>

#include "types.h"

enum InputAxis {
    INPUT_AXIS_VERTICAL = 0,
    INPUT_AXIS_HORIZONTAL = 1,
};

struct InputButtonState {
    bool isPressed;
    bool justPressed;
    bool justReleased;
};

struct InputState {
    struct InputButtonState button_up;
    struct InputButtonState button_left;
    struct InputButtonState button_right;
    struct InputButtonState button_down;

    struct InputButtonState button_x;
    struct InputButtonState button_c;
};

void update_input_state(struct InputState *inputs);

bool input_any_dir_pressed(const struct InputState *inputs);

bool input_get_pressed_direction(const struct InputState *inputs,
                                 enum InputAxis axis, enum Direction *out);

bool input_get_just_pressed_direction(const struct InputState *inputs,
                                      enum InputAxis axis, enum Direction *out);