#include <stdint.h>
#include "wasm4.h"


struct Player{
    uint32_t x;
    uint32_t y;
    
    uint32_t room_x;
    uint32_t room_y;
};


void handle_movement(struct Player *player);

void draw_player(const struct Player *player);