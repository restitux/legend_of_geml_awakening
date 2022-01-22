#pragma once

#include "block.h"
#include "types.h"

enum RoomState {
    ROOM_WATER = 0,
    ROOM_ICE = 1,
    ROOM_LAVA = 2,
    ROOM_NOTHING = 3,
};

struct RoomBlocks {
    struct Block b[32];
    uint32_t size;
};

struct Room {
    struct RoomCoordinate loc;

    struct RoomBlocks blocks;

    enum RoomState state;
};