#pragma once

#include "input.h"
#include "player.h"
#include "room.h"
#include "room_renderer.h"
#include "text.h"

#include <stdbool.h>

struct DefaultRoomStates {
    uint16_t id[3];
    uint8_t length[3];
    enum RoomState *state[3];
};

struct PlayerInventory {
    bool ice_to_water;
    bool lava_to_water;
};

extern struct DefaultRoomStates default_room_states;

struct GameState {
    bool valid;
    struct Player player;

    const struct TileMap *overworld;

    struct Room currentRoom;

    struct InputState inputs;
    struct TextState text_state;
    struct PlayerInventory inventory;

    struct DefaultRoomStates room_states;

    uint16_t world_id;
};

extern struct GameState game_state;

void init_game();

void save_game();

void load_game();

void update_current_room_state(struct GameState *game,
                               enum RoomState new_state);

void set_default_room_state(struct GameState *game);