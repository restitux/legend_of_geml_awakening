#pragma once

#include "../../res/map/tiled.h"
#include "input.h"
#include "room_renderer.h"
#include "sprite.h"
#include "types.h"
#include "wasm4.h"
#include <stdint.h>
struct Room;

struct Player {
    uint8_t layer;

    struct WorldCoordinate loc;
    struct CharacterSprite sprite;
    enum Direction last_movement_dir;

    uint32_t player_height;
    uint32_t player_width;
};

void handle_movement(struct Player *player, const struct TileMap *map,
                     const struct InputState *inputs, const struct Room *room);

void draw_player(const struct Player *player);
