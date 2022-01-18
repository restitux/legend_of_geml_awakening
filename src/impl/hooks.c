#include "hooks.h"
#include "game.h"
#include "room_renderer.h"
#include "wasm4.h"

#include "entrances.h"

void on_room_enter() {}

void on_room_exit() {}

void on_game_launch() {
    PALETTE[0] = 0x00FF0000;
    PALETTE[1] = 0x00000000;
    PALETTE[2] = 0x00666666;
    PALETTE[3] = 0x00FFFFFF;
    *DRAW_COLORS = 0x4320;
}

void on_update() {
    handle_movement(&game_state.player, &game_state.overworld->collision_map);
    room_draw_room(game_state.player.loc.room.x, game_state.player.loc.room.y,
                   &game_state.overworld->static_map);
    draw_player(&game_state.player);
    room_draw_room(game_state.player.loc.room.x, game_state.player.loc.room.y,
                   &game_state.overworld->overlay_map);
    // handle_entrances();
}
