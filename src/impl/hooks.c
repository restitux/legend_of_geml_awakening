#include "hooks.h"
#include "game.h"
#include "room_renderer.h"
#include "wasm4.h"

#include "block.h"
#include "configuration.h"
#include "entrances.h"

#include "collision.h"

void on_room_enter() {
    struct Room *new_room = &game_state.currentRoom;

    uint32_t spawn_count = 0;

    for (uint32_t i = 0; i < game_state.overworld->block_spawns.length; i++) {
        struct TileMap_BlockSpawn s =
            game_state.overworld->block_spawns.block_spawns[i];

        struct WorldCoordinate block_location =
            coordinate_global_to_world(s.x, s.y);

        if (block_location.room.x == new_room->loc.x &&
            block_location.room.y == new_room->loc.y) {
            spawn_count += 1;
        }
    }

    tracef("entering room (%d, %d) with %d (of %d) blocks", new_room->loc.x,
           new_room->loc.y, spawn_count,
           game_state.overworld->block_spawns.length);

    game_state.currentRoom.blocks.size = spawn_count;
    game_state.currentRoom.blocks.b =
        malloc(sizeof(struct Block) * spawn_count);
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
    tracef("finished loading the following blocks:");
    for (uint32_t i = 0; i < game_state.currentRoom.blocks.size; i++) {
        struct Block *b = &game_state.currentRoom.blocks.b[i];

        tracef("id: %d (index: %d); grid: %d, %d;", b->id, i, b->loc.x,
               b->loc.y);
    }
}

void on_room_exit() {
    tracef("freeing %d block(s) as I leave room %d %d",
           game_state.currentRoom.blocks.size, game_state.player.loc.room.x,
           game_state.player.loc.room.y);
    free(game_state.currentRoom.blocks.b);
    game_state.currentRoom.blocks.size = 0;
    save_game();

    game_state.currentRoom.loc = game_state.player.loc.room;
}

void on_game_launch() {
    PALETTE[0] = 0x000099CC;
    PALETTE[1] = 0x00000000;
    PALETTE[2] = 0x00999999;
    PALETTE[3] = 0x00FFFFFF;
    *DRAW_COLORS = 0x4320;

    load_game();

    on_room_enter();
}

bool is_animating = false;
struct BlockPushAnimation animation;

struct TerrainMap terrain;

void on_update() {
    room_draw_room(game_state.player.loc.room.x, game_state.player.loc.room.y,
                   &game_state.overworld->static_map);
    terrain_map_update(&terrain, &game_state.currentRoom.blocks,
                       game_state.currentRoom.loc, game_state.overworld);

    update_input_state(&game_state.inputs);

    handle_movement(&game_state.player, &terrain, &game_state.inputs);

    room_draw_room_special_tiles(game_state.player.loc.room.x,
                                 game_state.player.loc.room.y,
                                 &game_state.overworld->special_map);
    for (uint32_t i = 0; i < game_state.currentRoom.blocks.size; i++) {
        struct Block *b = &game_state.currentRoom.blocks.b[i];

        enum Direction d;
        if (!is_animating && block_is_push_attempted(&game_state.player, b,
                                                     &game_state.inputs, &d)) {
            block_push_begin(&game_state.player, b, d, &terrain, &animation);
            is_animating = true;
        }
        if (is_animating) {
            is_animating = block_push_step(&animation);
        }
        if (!is_animating) {
            block_update_layer(b, &terrain);
        }
        block_draw_block(b);
    }
    draw_player(&game_state.player);

    room_draw_room(game_state.player.loc.room.x, game_state.player.loc.room.y,
                   &game_state.overworld->overlay_map);
    ONLY_DEBUG(room_draw_room_debug_map(game_state.player.loc.room.x,
                                        game_state.player.loc.room.y,
                                        &game_state.overworld->collision_map));

    ONLY_DEBUG(room_draw_room_debug_map(game_state.player.loc.room.x,
                                        game_state.player.loc.room.y,
                                        &game_state.overworld->special_map));

    handle_entrances();
}
