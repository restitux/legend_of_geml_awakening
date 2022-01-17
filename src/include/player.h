#include "../../res/map/tiled.h"
#include "types.h"
#include "wasm4.h"
#include "sprite.h"
#include <stdint.h>

struct Player {
    struct WorldCoordinate loc;
    struct CharacterSprite sprite;
    enum Direction last_movement_dir;
};

void handle_movement(struct Player *player,
                     const struct TileMap_DataLayer *collision_map);

void draw_player(const struct Player *player);
