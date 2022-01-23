#include "wasm4.h"

#include "text.h"

#define TEXT_BOX_X 4
#define TEXT_BOX_Y 120
#define TEXT_BOX_WIDTH 152
#define TEXT_BOX_HEIGHT 36
#define TEXT_BOX_LETTER_X TEXT_BOX_X + 2
#define TEXT_BOX_LETTER_Y TEXT_BOX_Y + 2
#define TEXT_BOX_LETTER_WIDTH 8
#define TEXT_BOX_LETTER_HEIGHT 8

void render_text(struct TextState *state) {
    uint16_t old_draw_colors = *DRAW_COLORS;
    *DRAW_COLORS = 0x0023;
    // draw text box background
    rect(TEXT_BOX_X, TEXT_BOX_Y, TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT);
    state->counter++;
    if (state->counter == state->render_speed) {
        state->progress++;
        state->counter = 0;
    }

    *DRAW_COLORS = 0x0004;
    int letter_x = 0;
    int letter_y = 0;
    for (int i = 0; i < state->progress; i++) {
        if (i == state->current_text.length) {
            break;
        }

        char str[] = {state->current_text.text[i], '\0'};
        text(str, TEXT_BOX_LETTER_X + (TEXT_BOX_LETTER_WIDTH * letter_x),
             TEXT_BOX_LETTER_Y + (TEXT_BOX_LETTER_HEIGHT * letter_y));
        if (letter_x > 16) {
            letter_x = 0;
            letter_y++;
        } else {
            letter_x++;
        }
    }

    *DRAW_COLORS = old_draw_colors;
}
