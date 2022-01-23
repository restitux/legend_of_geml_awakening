#pragma once

#include "input.h"
#include "player.h"
#include "room.h"
#include "room_renderer.h"
#include "text.h"

struct DefaultRoomStates {
    uint16_t id[3];
    uint8_t length[3];
    enum RoomState *state[3];
};

extern struct DefaultRoomStates default_room_states;

struct GameState {
    bool valid;
    struct Player player;

    const struct TileMap *overworld;

    struct Room currentRoom;

    struct InputState inputs;
    struct TextState text_state;
};

extern struct GameState game_state;

void init_game();

void save_game();

void load_game();
