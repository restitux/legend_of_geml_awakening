#include "../res/map/tiled.h"
#include "types.h"
#include "wasm4.h"
#include <stdint.h>

struct Player {
    struct WorldCoordinate loc;
};

void handle_movement(struct Player *player,
                     const struct TileMap_DataLayer *collision_map);

void draw_player(const struct Player *player);
