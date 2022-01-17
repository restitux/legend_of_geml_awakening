#include "game.h"
#include "../../res/data/player_animation.h"

struct GameState global_game_state = {
    .player = {
        .loc = {.room = {0, 0}, .screen = {35, 70}}, 
        .sprite = {
            .sprite_sheet=player_animation,
            .sheet_height=player_animationHeight,
            .sheet_width=player_animationWidth,
            .frame_size=player_animationFrameSize,
            .current_frame=0,
            .frames_per_animation=10
        },
        .player_height = 16,
        .player_width = 16      
    },
};

