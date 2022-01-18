#include "entrances.h"

#include "../../res/map/tiled.h"
#include "game.h"
#include "player.h"

void handle_entrances() {
    for (unsigned int i = 0; i < game_state.overworld->entrances.length; i++) {
        struct TileMap_Entrance e =
            game_state.overworld->entrances.entrances[i];
        if (e.is_entrance) {
            // convert player coord to world coord
            uint32_t x = game_state.player.loc.room.x * 160 +
                         game_state.player.loc.screen.x;
            uint32_t y = game_state.player.loc.room.y * 160 +
                         game_state.player.loc.screen.y;
            // if player in entrance
            if (x >= e.x && x <= e.x + e.width && y >= e.y &&
                y <= e.y + e.height) {
                // do transition
                // game_state.overworld = e.target_map;
            }
        }
    }
}
