#include "hooks.h"
#include "game.h"
#include "room_renderer.h"
#include "wasm4.h"

#include "block.h"
#include "configuration.h"
#include "entrances.h"
#include "special_tile.h"

#include "collision.h"

void on_room_enter() {
    struct Room *new_room = &game_state.currentRoom;

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

bool is_animating = false;
struct BlockPushAnimation animation;

struct TerrainMap terrain;

void on_update() {
    set_palette(game_state.currentRoom.state);
    room_draw_room(game_state.player.loc.room.x, game_state.player.loc.room.y,
                   &game_state.overworld->static_map);
    terrain_map_update(&terrain, &game_state.currentRoom, game_state.overworld);

    update_input_state(&game_state.inputs);
    if (game_state.inputs.button_x.justPressed) {
        game_state.currentRoom.state += 1;
        game_state.currentRoom.state %= 3;
    }

    handle_movement(&game_state.player, &terrain, &game_state.inputs);

    room_draw_room_special_tiles(game_state.player.loc.room.x,
                                 game_state.player.loc.room.y,
                                 &game_state.overworld->special_map);
    for (uint32_t i = 0; i < game_state.currentRoom.blocks.size; i++) {
        struct Block *b = &game_state.currentRoom.blocks.b[i];

        enum Direction d;
        if (!is_animating && block_is_push_attempted(&game_state.player, b,
                                                     &game_state.inputs, &d)) {
            block_push_begin(&game_state.player, b, d, &terrain,
                             &game_state.inputs, &animation);
            is_animating = true;
        }
        if (is_animating) {
            is_animating = block_push_step(&animation, &terrain);
        }
        if (!is_animating) {
            block_update_layer(b, &terrain);
        }
    }

    block_draw_all_blocks(game_state.currentRoom.blocks.b,
                          game_state.currentRoom.blocks.size);

    draw_player(&game_state.player);

    if (animation.dir == DIRECTION_DOWN) {
        block_draw_block(animation.block);
    }

    room_draw_room(game_state.player.loc.room.x, game_state.player.loc.room.y,
                   &game_state.overworld->overlay_map);
    ONLY_DEBUG(room_draw_room_debug_map(game_state.player.loc.room.x,
                                        game_state.player.loc.room.y,
                                        &game_state.overworld->collision_map));

    ONLY_DEBUG(room_draw_room_debug_map(game_state.player.loc.room.x,
                                        game_state.player.loc.room.y,
                                        &game_state.overworld->special_map));

    handle_entrances();
    terrain_debug_draw(&terrain);
}
