#pragma once

#include <stdint.h>

struct Tilemap {
    const uint32_t *map;
    uint32_t map_width;
    uint32_t map_height;

    const uint8_t *tileset;
};

struct Datamap {
    const uint32_t *map;
    uint32_t map_width;
    uint32_t map_height;
};