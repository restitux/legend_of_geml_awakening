#include "game.h"
#include "../../res/data/player_animation.h"
#include "../../res/map/dungeon_one.map.h"
#include "../../res/map/dungeon_two.map.h"
#include "../../res/map/testing.map.h"
#include "../../res/map/tiled.h"
#include "wasm4.h"

struct GameState game_state;
struct DefaultRoomStates default_room_states = (struct DefaultRoomStates){
    .id = {TILEMAP_DUNGEON_ONE_ID, TILEMAP_DUNGEON_TWO_ID, TILEMAP_TESTING_ID},
    .length = {},
    .state = {(enum RoomState[]){ROOM_WATER, ROOM_WATER, ROOM_WATER, ROOM_WATER,
                                 ROOM_WATER, ROOM_WATER, ROOM_WATER, ROOM_WATER,
                                 ROOM_WATER, ROOM_ICE, ROOM_ICE, ROOM_ICE},
              (enum RoomState[]){ROOM_LAVA, ROOM_LAVA, ROOM_LAVA, ROOM_LAVA,
                                 ROOM_LAVA, ROOM_LAVA, ROOM_LAVA, ROOM_LAVA,
                                 ROOM_LAVA, ROOM_LAVA, ROOM_LAVA, ROOM_LAVA,
                                 ROOM_LAVA, ROOM_LAVA, ROOM_LAVA, ROOM_LAVA,
                                 ROOM_LAVA, ROOM_LAVA},
              (enum RoomState[]){ROOM_WATER, ROOM_WATER, ROOM_WATER, ROOM_WATER,
                                 ROOM_WATER, ROOM_WATER, ROOM_WATER, ROOM_WATER,
                                 ROOM_WATER, ROOM_WATER, ROOM_WATER,
                                 ROOM_WATER}},
};

void init_game() {
    game_state = (struct GameState){
        .valid = true,
        .player = {.layer = 0,
                   .loc = {.room = {0, 0}, .screen = {35, 100}},
                   .sprite = {.sprite_sheet = player_animation,
                              .sheet_height = player_animationHeight,
                              .sheet_width = player_animationWidth,
                              .frame_size = player_animationFrameSize,
                              .current_frame = 0,
                              .frames_per_animation = 10},
                   .player_height = 16,
                   .player_width = 16},
        .overworld = &testing_tilemap,
        .world_id = TILEMAP_TESTING_ID,
        .currentRoom =
            {
                .loc = {.x = 0, .y = 0},
                .blocks =
                    {
                        .size = 0,
                    },
                .state = ROOM_WATER,
            },
        .text_state =
            (struct TextState){
                .displaying = false,
                .render_speed = 10,
                .current_text =
                    (struct TextBox){
                        .text =
                            "Hello and welcome to Gamelon. My name is Shitty "
                            "Wizard",
                        .length = 54,
                    },
                .progress = 0,
                .counter = 0,
                .remain_on_screen_time = 120,
            },
        .inventory =
            (struct PlayerInventory){
                .ice_to_water = false,
                .lava_to_water = false,
            },
        .room_states = default_room_states,
    };
}

void save_game() {
    // diskw(&game_state, sizeof(struct GameState));
}

void load_game() {
    // struct GameState temp;
    // diskr(&temp, sizeof(struct GameState));
    // if (temp.valid) {
    //   game_state = temp;
    // }
}

int calc_room_id(struct GameState *game) {
    return game->currentRoom.loc.x +
           (game->currentRoom.loc.y * (game->overworld->static_map.width / 20));
}

void update_current_room_state(struct GameState *game,
                               enum RoomState new_state) {

    game->currentRoom.state = new_state;

    if (game->world_id == TILEMAP_TESTING_ID) { // overworld
        trace("updating overwold room state");
        int room_id = calc_room_id(game);
        game->room_states.state[2][room_id] = new_state;
    }
}

void set_default_room_state(struct GameState *game) {
    int state_array_loc = 0;
    for (uint8_t i = 0; i < 3; i++) {
        if (game->room_states.id[i] == game->world_id) {
            state_array_loc = i;
        }
    }

    int room_id = calc_room_id(game);

    enum RoomState new_state =
        game->room_states.state[state_array_loc][room_id];

    game->currentRoom.state = new_state;
}