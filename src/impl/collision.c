#include "collision.h"
#include "wasm4.h"

void clear_terrain(struct TerrainMap *t) {
    for (int i = 0; i < 400; i++) {
        t->terrain[i] = 0;
    }
}

uint32_t compute_terrain_index(uint32_t x, uint32_t y) { return x + y * 20; }

void terrain_debug_draw(const struct TerrainMap *t) {
    for (uint32_t x = 0; x < 20; x++) {
        for (uint32_t y = 0; y < 20; y++) {
            uint32_t index = compute_terrain_index(x, y);
            enum TerrianType type = terrain_type(t->terrain[index]);
            enum TerrainLayer layer = terrain_layer(t->terrain[index]);

            if (type == TERRAIN_WALL && layer == LAYER_MAIN) {
                text("X", x * 8, y * 8);
            } else if (type == TERRAIN_BLOCK && layer == LAYER_MAIN) {
                text("b", x * 8, y * 8);
            }
        }
    }
}

void copy_tilemap_state(struct TerrainMap *t, struct RoomCoordinate loc,
                        const struct TileMap *map) {
    uint32_t map_width = map->static_map.width;
    int room_start_x = loc.x * 20;
    int room_start_y = loc.y * 20;
    for (uint32_t x = 0; x < 20; x++) {
        for (uint32_t y = 0; y < 20; y++) {
            int bit_index = room_start_x + x + ((room_start_y + y) * map_width);
            int byte_index = bit_index / 8;
            int bit_offset = (bit_index) % 8;

            int terrain_index = compute_terrain_index(x, y);

            uint8_t collision_byte = map->collision_map.map[byte_index];
            uint8_t collision_bit = (collision_byte >> bit_offset) & 0x1;

            uint8_t special_byte = map->special_map.map[byte_index];
            uint8_t special_bit = (special_byte >> bit_offset) & 0x1;

            t->terrain[terrain_index] |=
                ((special_bit << 4) | (special_bit << 1));

            t->terrain[terrain_index] |= collision_bit;
        }
    }
}

void terrain_map_update(struct TerrainMap *t, struct RoomBlocks *blocks,
                        struct RoomCoordinate loc, const struct TileMap *map) {
    clear_terrain(t);
    copy_tilemap_state(t, loc, map);

    for (uint32_t i = 0; i < blocks->size; i++) {
        struct Block *block = &blocks->b[i];

        enum TerrainLayer layer = block->layer;

        Terrain terrain = 4;

        for (int x = block->loc.x; x < block->loc.x + 2; x++) {
            for (int y = block->loc.y; y < block->loc.y + 2; y++) {
                int terrain_index = compute_terrain_index(x, y);
                t->terrain[terrain_index] = terrain;
            }
        }
    }
}

uint8_t terrain_at_point(const struct TerrainMap *t,
                         struct ScreenCoordinate p) {
    struct GridCoordinate g = coordinate_screen_to_grid(&p);
    rect(g.x * 8, g.y * 8, 1, 1);
    if (g.x >= 20 || g.y >= 20) {
        return 0;
    }
    return t->terrain[compute_terrain_index(g.x, g.y)];
}

uint8_t terrain_type(Terrain terrain) { return (terrain & 0x0F); }

uint8_t terrain_layer(Terrain terrain) { return (terrain >> 4) & 0x0F; }