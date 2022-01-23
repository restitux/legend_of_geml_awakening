#include "game.h"
#include "wasm4.h"

#define VALID 0xCAFEBABE

struct SaveGame {
    uint32_t valid;
    struct Player player;
    struct PlayerInventory inventory;
    struct Room currentRoom;
    uint16_t world_id;
};

void save();
bool load_save_game();