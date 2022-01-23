#include "wasm4.h"

#include "text.h"

#include "inventory.h"

#define TEXT_BOX_X 4
#define TEXT_BOX_Y 120
#define TEXT_BOX_WIDTH 152
#define TEXT_BOX_HEIGHT 36
#define TEXT_BOX_LETTER_X TEXT_BOX_X + 2
#define TEXT_BOX_LETTER_Y TEXT_BOX_Y + 2
#define TEXT_BOX_LETTER_WIDTH 8
#define TEXT_BOX_LETTER_HEIGHT 8

void handle_text_triggers(struct TextState *state,
                          struct TileMap_TextTriggers triggers,
                          struct Player player) {
    // only look for new triggers if no trigger is activated
    if (!state->displaying) {
        for (unsigned int i = 0; i < triggers.length; i++) {
            struct TileMap_TextTrigger t = triggers.text_triggers[i];
            if (t.id == state->current_text.id) {
                continue;
            }
            // convert player coord to world coord
            uint32_t x = player.loc.room.x * 160 + player.loc.screen.x;
            uint32_t y = player.loc.room.y * 160 + player.loc.screen.y;
            if (x >= t.x && x <= t.x + t.width && y >= t.y &&
                y <= t.y + t.height) {
                state->displaying = true;
                state->current_text = t;

                if (t.ability_pickup != -1) {
                    unlock_ability(t.ability_pickup);
                }
            }
        }
    }
}

void render_text(struct TextState *state) {
    // draw text box background
    state->counter++;
    state->progress = state->counter / state->render_speed;

    // if we are done rendering, unset displaying
    uint16_t max_duration = state->remain_on_screen_time +
                            (state->current_text.length * state->render_speed);
    if (state->counter >= max_duration) {
        state->displaying = false;
        state->counter = 0;
        state->progress = 0;
        return;
    }

    uint16_t old_draw_colors = *DRAW_COLORS;
    *DRAW_COLORS = 0x0023;
    rect(TEXT_BOX_X, TEXT_BOX_Y, TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT);

    *DRAW_COLORS = 0x0004;
    int letter_x = 0;
    int letter_y = 0;
    for (int i = 0; i < state->progress; i++) {
        if (i == state->current_text.length) {
            break;
        }

        char str[] = {state->current_text.string[i], '\0'};
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
