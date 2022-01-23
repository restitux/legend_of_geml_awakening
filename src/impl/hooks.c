#include "hooks.h"
#include "game.h"
#include "room_renderer.h"
#include "wasm4.h"

#include "block.h"
#include "configuration.h"
#include "entrances.h"
#include "special_tile.h"
#include "text.h"

#include "collision.h"

void on_room_enter() {
    struct Room *new_room = &game_state.currentRoom;

    set_default_room_state(&game_state);

    uint32_t index = 0;

    for (uint32_t i = 0; i < game_state.overworld->block_spawns.length; i++) {
        struct TileMap_BlockSpawn s =
            game_state.overworld->block_spawns.block_spawns[i];

        struct WorldCoordinate block_location =
            coordinate_global_to_world(s.x, s.y);

        if (block_location.room.x == new_room->loc.x &&
            block_location.room.y == new_room->loc.y) {
            struct GridCoordinate g =
                coordinate_screen_to_grid(&block_location.screen);

            block_new(g, &game_state.currentRoom.blocks.b[index]);
            game_state.currentRoom.blocks.b[index].id = s.id;
            tracef("loaded block %d at (%d, %d)", s.id, block_location.screen.x,
                   block_location.screen.y);
            index += 1;
        }
    }
    game_state.currentRoom.blocks.size = index;
}

void on_room_exit() {
    tracef("freeing %d block(s) as I leave room %d %d",
           game_state.currentRoom.blocks.size, game_state.player.loc.room.x,
           game_state.player.loc.room.y);
    game_state.currentRoom.blocks.size = 0;
    save_game();

    game_state.currentRoom.loc = game_state.player.loc.room;
}

void on_game_launch() {

    load_game();

    on_room_enter();
}

struct TerrainMap terrain;

void on_update() {
    set_palette(game_state.currentRoom.state);
    room_draw_room_special_tiles(game_state.player.loc.room.x,
                                 game_state.player.loc.room.y,
                                 &game_state.overworld->special_map, &terrain,
                                 game_state.currentRoom.state);
    room_draw_room(game_state.player.loc.room.x, game_state.player.loc.room.y,
                   &game_state.overworld->static_map);
    terrain_map_update(&terrain, &game_state.currentRoom, game_state.overworld);

    update_input_state(&game_state.inputs);
    if (game_state.inputs.button_x.justPressed) {
        enum RoomState new_state = game_state.currentRoom.state + 1;
        new_state %= 3;
        update_current_room_state(&game_state, new_state);
    }

    handle_movement(&game_state.player, &terrain, &game_state.inputs);

    block_update_all_blocks(game_state.currentRoom.blocks.b,
                            game_state.currentRoom.blocks.size,
                            &game_state.player, &game_state.inputs, &terrain);
    if (game_state.currentRoom.state == ROOM_ICE) {
        for (uint32_t i = 0; i < game_state.currentRoom.blocks.size; i++) {
            block_set_layer(&game_state.currentRoom.blocks.b[i], LAYER_MAIN);
        }
    }

    block_draw_all_blocks(game_state.currentRoom.blocks.b,
                          game_state.currentRoom.blocks.size);

    draw_player(&game_state.player);

    // Overlay map uses run length encoding strategy
    room_draw_room_rle(game_state.player.loc.room.x,
                       game_state.player.loc.room.y,
                       &game_state.overworld->overlay_map);
    ONLY_DEBUG(room_draw_room_debug_map(game_state.player.loc.room.x,
                                        game_state.player.loc.room.y,
                                        &game_state.overworld->collision_map));

    ONLY_DEBUG(room_draw_room_debug_map(game_state.player.loc.room.x,
                                        game_state.player.loc.room.y,
                                        &game_state.overworld->special_map));

    handle_entrances();
    // terrain_debug_draw(&terrain);
    // render_text(&game_state.text_state);
}
