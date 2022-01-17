#include "tilemap.h"
#include <stdint.h>
#include "types.h"

void room_draw_room(uint32_t room_x, uint32_t room_y, const struct Tilemap *map);

int room_tile_at_screen_coordinates(struct WorldCoordinate *loc, const struct Datamap *map);