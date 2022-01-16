#include "player.h"
#include "wasm4.h"

void handle_movement(struct Player *player) {
    uint8_t gamepad = *GAMEPAD1;

    if (gamepad & BUTTON_UP) {
        player->y -= 1;
    }
    if (gamepad & BUTTON_DOWN) {
        player->y += 1;
    }
    if (gamepad & BUTTON_LEFT) {
        player->x -= 1;
    }
    if (gamepad & BUTTON_RIGHT) {
        player->x += 1;
    }
}

void draw_player(const struct Player *player) {
    text("P", player->x, player->y);
}