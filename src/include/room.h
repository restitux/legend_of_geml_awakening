#pragma once

#include "block.h"
#include "types.h"

struct RoomBlocks {
    struct Block b[32];
    uint32_t size;
};

struct Room {
    struct RoomCoordinate loc;

    struct RoomBlocks blocks;
};