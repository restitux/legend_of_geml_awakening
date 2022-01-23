#ifndef __TEXT_H_
#define __TEXT_H_

#include <stdbool.h>
#include <stdint.h>

struct TextBox {
    const char *text;
    uint16_t length;
};

struct TextState {
    bool displaying;
    uint16_t progress;
    uint16_t render_speed;
    uint16_t counter;
    struct TextBox current_text;
};

void render_text(struct TextState *state);

#endif // __TEXT_H_
