#include "hooks.h"

//call out to game hooks


void start() {
    on_game_launch();
}

void update() {
    on_update();
}
