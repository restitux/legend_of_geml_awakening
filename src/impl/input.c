#include "input.h"
#include "wasm4.h"

void update_button(struct InputButtonState *b, bool cur_state) {
    struct InputButtonState old = *b;

    b->justReleased = false;
    b->justPressed = false;
    b->isPressed = false;

    if (cur_state) {
        b->isPressed = true;
    }

    if (!old.isPressed && b->isPressed) {
        b->justPressed = true;
    }
    if (old.isPressed && !b->isPressed) {
        b->justReleased = true;
    }
}

void update_input_state(struct InputState *inputs) {
    uint8_t gamepad = *GAMEPAD1;

    update_button(&inputs->button_up, gamepad & BUTTON_UP);

    update_button(&inputs->button_down, gamepad & BUTTON_DOWN);

    update_button(&inputs->button_left, gamepad & BUTTON_LEFT);

    update_button(&inputs->button_right, gamepad & BUTTON_RIGHT);

    update_button(&inputs->button_x, gamepad & BUTTON_1);

    update_button(&inputs->button_c, gamepad & BUTTON_2);
}

bool input_any_dir_pressed(const struct InputState *inputs) {
    return (inputs->button_up.isPressed || inputs->button_down.isPressed ||
            inputs->button_left.isPressed || inputs->button_right.isPressed);
}

bool input_any_dir_just_pressed(const struct InputState *inputs) {
    return (inputs->button_up.justPressed || inputs->button_down.justPressed ||
            inputs->button_left.justPressed ||
            inputs->button_right.justPressed);
}

bool input_get_pressed_direction(const struct InputState *inputs,
                                 enum InputAxis axis, enum Direction *out) {
    if (axis == INPUT_AXIS_VERTICAL) {
        bool button_up = inputs->button_up.isPressed;
        bool button_down = inputs->button_down.isPressed;

        if (button_up && button_down) {
            return false;
        }
        if (!button_up && !button_down) {
            return false;
        }

        if (button_up) {
            *out = DIRECTION_UP;
        }
        if (button_down) {
            *out = DIRECTION_DOWN;
        }
        return true;
    }
    if (axis == INPUT_AXIS_HORIZONTAL) {
        bool button_left = inputs->button_left.isPressed;
        bool button_right = inputs->button_right.isPressed;

        if (button_left && button_right) {
            return false;
        }
        if (!button_left && !button_right) {
            return false;
        }

        if (button_left) {
            *out = DIRECTION_LEFT;
        }
        if (button_right) {
            *out = DIRECTION_RIGHT;
        }
        return true;
    }
    return false;
}

bool input_get_just_pressed_direction(const struct InputState *inputs,
                                      enum InputAxis axis,
                                      enum Direction *out) {
    if (axis == INPUT_AXIS_VERTICAL) {
        bool button_up = inputs->button_up.justPressed;
        bool button_down = inputs->button_down.justPressed;

        if (button_up && button_down) {
            return false;
        }
        if (!button_up && !button_down) {
            return false;
        }

        if (button_up) {
            *out = DIRECTION_UP;
        }
        if (button_down) {
            *out = DIRECTION_DOWN;
        }
        return true;
    }
    if (axis == INPUT_AXIS_HORIZONTAL) {
        bool button_left = inputs->button_left.justPressed;
        bool button_right = inputs->button_right.justPressed;

        if (button_left && button_right) {
            return false;
        }
        if (!button_left && !button_right) {
            return false;
        }

        if (button_left) {
            *out = DIRECTION_LEFT;
        }
        if (button_right) {
            *out = DIRECTION_RIGHT;
        }
        return true;
    }
    return false;
}