#include "game.h" //main must include game.h for the state to be initalized
#include "hooks.h"
void start() {
    on_game_launch();
}

void update() {
    on_update();
}
