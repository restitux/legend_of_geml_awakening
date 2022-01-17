#include "../../res/map/tiled.h"
#include "types.h"
#include <stdint.h>

struct Room {
    struct RoomCoordinate loc;
    const struct TileMap *map;
};

void room_draw_room(uint32_t room_x, uint32_t room_y,
                    const struct TileMap_MapLayer *map);

int room_tile_at_screen_coordinates(struct WorldCoordinate *loc,
                                    const struct TileMap_DataLayer *map);

