#include "inventory.h"

void draw_inventory(struct PlayerInventory *inventory) {
    uint16_t old_colors = *DRAW_COLORS;
    if (inventory) {
    }
    *DRAW_COLORS = 0x0024;
    rect(32, 160 - 48, 160 - 64, 32);

    if (inventory->ice_to_water) {
        text("W->I", 32 + 8, 160 - 48 + 8);
    }

    if (inventory->lava_to_water) {
        text("L->W", 80 + 8, 160 - 48 + 8);
    }
    *DRAW_COLORS = 0x0020;
    if (inventory->selected_ability == ICE_TO_WATER) {

        rect(32 + 8 - 2, 160 - 48 + 8 - 2, 8 * 4 + 4, 8 + 4);
    }
    if (inventory->selected_ability == LAVA_TO_WATER) {
        rect(80 + 8 - 2, 160 - 48 + 8 - 2, 8 * 4 + 4, 8 + 4);
    }

    *DRAW_COLORS = old_colors;
}

bool handle_invetory_interaction(struct GameState *game) {
    struct PlayerInventory *inventory = &game->inventory;
    struct InputState *i = &game->inputs;
    if (!inventory->ice_to_water && !inventory->lava_to_water) {
        return false;
    }

    if (i->button_x.isPressed) {
        if (i->button_left.justPressed) {
            inventory->selected_ability -= 1;
        }
        if (i->button_right.justPressed) {
            inventory->selected_ability += 1;
        }
        inventory->selected_ability %= 2;
        return true;
    }

    if (i->button_x.justReleased) {
        enum RoomState current_state = game->currentRoom.state;
        if (inventory->selected_ability == ICE_TO_WATER &&
            inventory->ice_to_water && current_state == ROOM_WATER) {
            update_current_room_state(game, ROOM_ICE);
        } else if (inventory->selected_ability == LAVA_TO_WATER &&
                   inventory->lava_to_water && current_state == ROOM_LAVA) {
            update_current_room_state(game, ROOM_WATER);
        }
    }

    return false;
}