#include "player.h"
#include "../../res/map/tiled.h"
#include "configuration.h"
#include "hooks.h"
#include "room_renderer.h"
#include "sprite.h"
#include "types.h"
#include "wasm4.h"

void move_player_if_valid2(struct Player *player, enum Direction direction,
                           const struct TileMap_DataLayer *collision_map) {
  struct BoundingBox bb =
      bounding_box_new(player->loc.screen, 16 - PLAYER_COLLISION_BUFFER * 2,
                       16 - PLAYER_COLLISION_BUFFER * 2);

  bb.tl.x += PLAYER_COLLISION_BUFFER;
  bb.tl.y += PLAYER_COLLISION_BUFFER;
  struct WorldCoordinate potential_loc = player->loc;
  switch (direction) {
  case DIRECTION_UP:
    bb.tl.y -= 1;
    potential_loc.screen.y -= 1;
    break;

  case DIRECTION_DOWN:
    bb.tl.y += 1;
    potential_loc.screen.y += 1;
    break;

  case DIRECTION_LEFT:
    bb.tl.x -= 1;
    potential_loc.screen.x -= 1;
    break;

  case DIRECTION_RIGHT:
    bb.tl.x += 1;
    potential_loc.screen.x += 1;
    break;
  default:
    break;
  }
  struct ScreenCoordinate bb_corners[4];
  bounding_box_corners(&bb, bb_corners);
  int tile = 0;
  for (int i = 0; i < 4; i++) {
    struct WorldCoordinate w = (struct WorldCoordinate){
        .screen = bb_corners[i],
        .room = potential_loc.room,
    };
    tile |= room_tile_at_screen_coordinates(&w, collision_map);
  }

  if (!tile) {

    if (potential_loc.screen.y < player->loc.screen.y) {
      player->last_movement_dir = DIRECTION_UP;
    } else if (potential_loc.screen.y > player->loc.screen.y) {
      player->last_movement_dir = DIRECTION_DOWN;
    }

    if (potential_loc.screen.x < player->loc.screen.x) {
      player->last_movement_dir = DIRECTION_LEFT;
    } else if (potential_loc.screen.x > player->loc.screen.x) {
      player->last_movement_dir = DIRECTION_RIGHT;
    }

    player->loc = potential_loc;
    sprite_advance_animation(&player->sprite);
  }
}

bool check_room_change(struct Player *player) {
  bool room_change = false;
  if (player->loc.screen.x > 159) {
    player->loc.screen.x = 1;
    player->loc.room.x += 1;
    room_change = true;
  }
  if (player->loc.screen.x < 1) {
    player->loc.screen.x = 159;
    player->loc.room.x -= 1;
    room_change = true;
  }
  if (player->loc.screen.y > 159) {
    player->loc.screen.y = 1;
    player->loc.room.y += 1;
    room_change = true;
  }
  if (player->loc.screen.y < 1) {
    player->loc.screen.y = 159;
    player->loc.room.y -= 1;
    room_change = true;
  }
  return room_change;
}

void handle_movement(struct Player *player,
                     const struct TileMap_DataLayer *collision_map,
                     const struct InputState *inputs) {
  uint8_t gamepad = *GAMEPAD1;

  if (inputs->button_up.isPressed) {
    move_player_if_valid2(player, DIRECTION_UP, collision_map);
  }
  if (inputs->button_down.isPressed) {
    move_player_if_valid2(player, DIRECTION_DOWN, collision_map);
  }
  if (inputs->button_left.isPressed) {
    move_player_if_valid2(player, DIRECTION_LEFT, collision_map);
  }
  if (inputs->button_right.isPressed) {
    move_player_if_valid2(player, DIRECTION_RIGHT, collision_map);
  }

  if (check_room_change(player)) {
    on_room_exit();
    on_room_enter();
  }
}

void draw_player(const struct Player *player) {
  sprite_draw_character(&player->sprite, &player->loc.screen,
                        player->last_movement_dir);
}
