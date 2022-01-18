#pragma once

#include <stdbool.h>

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