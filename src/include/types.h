#pragma once

#include <stdint.h>

struct ScreenCoordinate {
    uint32_t x;
    uint32_t y;
};

struct RoomCoordinate {
    uint32_t x;
    uint32_t y;
};

struct WorldCoordinate {
    struct RoomCoordinate room;
    struct ScreenCoordinate screen;
};

enum Direction {
    DIRECTION_UP = 0,
    DIRECTION_DOWN = 1,
    DIRECTION_LEFT = 2,
    DIRECTION_RIGHT = 3,
};

