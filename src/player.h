#include <stdint.h>
#include "wasm4.h"
#include "types.h"
#include "tilemap.h"

struct Player{
    struct WorldCoordinate loc;
};


void handle_movement(struct Player *player, struct Datamap *collision_map);

void draw_player(const struct Player *player);