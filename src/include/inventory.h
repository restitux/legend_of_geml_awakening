#include "game.h"
#include "input.h"
#include "room.h"
#include "wasm4.h"

void draw_inventory(struct PlayerInventory *inventory);

bool handle_invetory_interaction(struct GameState *game);

void draw_inventory(struct PlayerInventory *inventory);