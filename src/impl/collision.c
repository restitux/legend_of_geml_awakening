#include "collision.h"
#include "wasm4.h"

void clear_terrain(struct TerrainMap *t) {
    for (int i = 0; i < 400; i++) {
        t->terrain[i] = (TERRAIN_INVALID << 4);
    }
}

uint32_t compute_terrain_index(uint32_t x, uint32_t y) { return x + y * 20; }

void terrain_debug_draw(const struct TerrainMap *t) {
    for (uint32_t x = 0; x < 20; x++) {
        for (uint32_t y = 0; y < 20; y++) {
            uint32_t index = compute_terrain_index(x, y);

            enum TerrianType main = terrain_type(t->terrain[index], LAYER_MAIN);
            enum TerrianType lower =
                terrain_type(t->terrain[index], LAYER_LOWER);

            if (main == TERRAIN_WALL) {
                text("W", x * 8, y * 8);
            }
            if (main == TERRAIN_INVALID) {
                text("G", x * 8, y * 8);
            }
            if (main == TERRAIN_INVALID && lower == TERRAIN_BLOCK) {
                text("S", x * 8, y * 8);
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

            if (special_bit) {
                t->terrain[terrain_index] = TERRAIN_INVALID;
                // terrain_create(TERRAIN_NORMAL, LAYER_LOWER) |
                // terrain_create(TERRAIN_INVALID, LAYER_MAIN);
            }
            if (collision_bit) {
                t->terrain[terrain_index] =
                    terrain_create(TERRAIN_WALL, LAYER_MAIN) |
                    terrain_create(TERRAIN_INVALID, LAYER_LOWER);
            }
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

        Terrain terrain = terrain_create(TERRAIN_BLOCK, layer);

        for (int x = block->loc.x; x < block->loc.x + 2; x++) {
            for (int y = block->loc.y; y < block->loc.y + 2; y++) {
                int terrain_index = compute_terrain_index(x, y);
                Terrain cur_terrain = t->terrain[terrain_index];

                Terrain new_terrain = (cur_terrain & ~(0xF << layer)) | terrain;

                t->terrain[terrain_index] = new_terrain;
                if (layer == LAYER_LOWER) {
                    t->terrain[terrain_index] =
                        terrain_type(cur_terrain, LAYER_MAIN) |
                        terrain_create(TERRAIN_BLOCK, LAYER_LOWER);
                }
            }
        }
    }
}

uint8_t terrain_at_point(const struct TerrainMap *t,
                         struct ScreenCoordinate p) {
    struct GridCoordinate g = coordinate_screen_to_grid(&p);
    if (g.x >= 20 || g.y >= 20) {
        return 0;
    }
    return t->terrain[compute_terrain_index(g.x, g.y)];
}

uint8_t terrain_type(Terrain terrain, enum TerrainLayer layer) {
    if (layer > 1) {
        return TERRAIN_INVALID;
    }
    return (terrain >> (layer * 4)) & 0xF;
}

Terrain terrain_create(enum TerrianType t, enum TerrainLayer l) {
    return (t << (l * 4));
}