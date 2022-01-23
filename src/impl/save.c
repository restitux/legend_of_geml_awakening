#include "save.h"

void save() {
    struct SaveGame g = (struct SaveGame){
        .valid = VALID,
        .player = game_state.player,
        .inventory = game_state.inventory,
        .currentRoom = game_state.currentRoom,
        .world_id = game_state.world_id,
    };

    diskw(&g, sizeof(struct SaveGame));
}

bool load_save_game() {
    struct SaveGame g;
    diskr(&g, sizeof(struct SaveGame));

    if (g.valid == VALID) {
        game_state.currentRoom = g.currentRoom;
        game_state.player = g.player;
        game_state.inventory = g.inventory;
        game_state.world_id = g.world_id;

        return true;
    }

    return false;
}