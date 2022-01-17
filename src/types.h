#pragma once

#include <stdint.h>

struct ScreenCoordinate {
    uint32_t x;
    uint32_t y;
};

struct RoomCoordinate
{
    uint32_t x;
    uint32_t y;
};

struct WorldCoordinate
{
    struct RoomCoordinate room;
    struct ScreenCoordinate screen;
};

