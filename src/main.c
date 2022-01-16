#include <stdbool.h>

#include "wasm4.h"

#include "tile_renderer.h"

#include "../res/map/testing.map.h"
#include "../res/map/tiles.set.h"

#include "player.h"
#include "tilemap.h"
#include "room_renderer.h"

const unsigned int SCREEN_W = 160;
const unsigned int SCREEN_H = 160;

// const uint8_t tilemap[] = TILESET_tiles;
// const uint32_t tilesize = TILESIZE_tiles;

struct PlayerData {
  unsigned int x;
  unsigned int y;
};

// struct GameState {
//   struct PlayerData player_data;
//   const uint32_t *tilemap;
//   const uint8_t *tileset;
// } state = {
//     .player_data =
//         {
//             .x = 0,
//             .y = 0,
//         },
//     .tilemap = testing_tilemap,
//     .tileset = tiles_tileset,
// };

struct Player player = {80, 80, 0, 0};
struct Tilemap map = {testing_tilemap, 40, 20, tiles_tileset};

void update() {
  PALETTE[0] = 0x00FF0000;
  PALETTE[1] = 0x00000000;
  PALETTE[2] = 0x00444444;
  PALETTE[3] = 0x00FFFFFF;
  *DRAW_COLORS = 0x4321;
 
  
  handle_movement(&player);
  draw_player(&player);
  room_draw_room(1, 0, &map);

}
