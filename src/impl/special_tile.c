#include "special_tile.h"

#include "room.h"
#include "wasm4.h"

#define PALETTE_WATER 0x000099CC
#define PALETTE_LAVA 0x00de5849
#define PALETTE_ICE 0x00A4FCFA

#define BRIGHT_COLOR_INDEX 3
#define MID_COLOR_INDEX 2
#define DARK_COLOR_INDEX 1
#define SPECIAL_COLOR_INDEX 0

void set_palette(enum RoomState state) {
    PALETTE[SPECIAL_COLOR_INDEX] = 0x000099CC;
    PALETTE[DARK_COLOR_INDEX] = 0x1e331e;
    PALETTE[MID_COLOR_INDEX] = 0x90cf8f;    // 0x00999999;
    PALETTE[BRIGHT_COLOR_INDEX] = 0xf1f2e1; // 0x00FFFFFF;
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