#include "game.h"
#include "../../res/data/player_animation.h"
#include "../../res/map/dungeon_one.map.h"
#include "../../res/map/testing.map.h"
#include "../../res/map/tiled.h"
#include "wasm4.h"

struct GameState game_state;
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

        .currentRoom =
            {
                .loc = {.x = 0, .y = 0},
                .blocks =
                    {
                        .size = 0,
                    },
                .state = ROOM_WATER,
            },
        .text_state = (struct TextState){
            .displaying = false,
            .render_speed = 10,
            .current_text =
                (struct TextBox){
                    .text = "Hello and welcome to Gamelon. My name is Shitty "
                            "Wizard",
                    .length = 54,
                },
            .progress = 0,
            .counter = 0,
        }};
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
