#pragma once


struct InputButtonState {
    bool isPressed;
    bool justPressed;
    bool justReleased;
};

struct InputState {
    InputButtonState button_up;
    InputButtonState button_left;
    InputButtonState button_right;
    InputButtonState button_down;

    InputButtonState button_x;
    InputButtonState button_c;
};