#include "special_tile.h"

#include "room.h"
#include "wasm4.h"

#define PALETTE_WATER 0x000099CC
#define PALETTE_LAVA 0x00de5849
#define PALETTE_ICE 0x00A4FCFA

void set_palette(enum RoomState state) {
    PALETTE[0] = 0x000099CC;
    PALETTE[1] = 0x00000000;
    PALETTE[2] = 0x00999999;
    PALETTE[3] = 0x00FFFFFF;
    *DRAW_COLORS = 0x4320;

    switch (state) {
    case ROOM_WATER:
        PALETTE[0] = PALETTE_WATER;
        break;
    case ROOM_ICE:
        PALETTE[0] = PALETTE_ICE;
        break;
    case ROOM_LAVA:
        PALETTE[0] = PALETTE_LAVA;
        break;
    case ROOM_NOTHING:
        PALETTE[0] = 0x0;
        break;

    default:
        break;
    }
}