#include "hooks.h"
#include "game.h"
#include "room_renderer.h"
#include "wasm4.h"

#include "block.h"
#include "entrances.h"

void on_room_enter() {
  game_state.currentRoom.blocks.b = malloc(sizeof(struct Block) * 1);
  game_state.currentRoom.blocks.size = 1;

  block_new((struct GridCoordinate){8, 8}, game_state.currentRoom.blocks.b);
}

void on_room_exit() { save_game(); }

void on_game_launch() {
  PALETTE[0] = 0x00FF0000;
  PALETTE[1] = 0x00000000;
  PALETTE[2] = 0x00666666;
  PALETTE[3] = 0x00FFFFFF;
  *DRAW_COLORS = 0x4320;

  load_game();

  on_room_enter();
}

bool is_animating = false;
struct BlockPushAnimation animation;

void on_update() {
  update_input_state(&game_state.inputs);

  handle_movement(&game_state.player, &game_state.overworld->collision_map,
                  &game_state.inputs);
  room_draw_room(game_state.player.loc.room.x, game_state.player.loc.room.y,
                 &game_state.overworld->static_map);

  for (uint32_t i = 0; i < game_state.currentRoom.blocks.size; i++) {
    struct Block *b = &game_state.currentRoom.blocks.b[i];

    enum Direction d;
    if (!is_animating && block_is_push_attempted(&game_state.player, b,
                                                 &game_state.inputs, &d)) {
      tracef("starting block push in %d", d);
      block_push_begin(&game_state.player, b, d, &animation);
      is_animating = true;
    }

    if (is_animating) {
      is_animating = block_push_step(&animation);
      block_draw_block_push(&animation);
    } else {
      block_draw_block_static(b);
    }
  }
  draw_player(&game_state.player);

  room_draw_room(game_state.player.loc.room.x, game_state.player.loc.room.y,
                 &game_state.overworld->overlay_map);
  handle_entrances();
}
