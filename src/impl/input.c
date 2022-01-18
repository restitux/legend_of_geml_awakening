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
