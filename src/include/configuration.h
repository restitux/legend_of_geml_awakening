#pragma once

#include <stdint.h>

//-------------------------//
// GAME CONFIGURATION FLAGS//
//-------------------------//

// DEBUG
// #define DEBUG_DRAWS

// #define BLOCK_DEBUG_DRAW_ID

#ifdef DEBUG_DRAWS
#define ONLY_DEBUG(expr)                                                       \
    do {                                                                       \
        expr;                                                                  \
    } while (0)
#else
#define ONLY_DEBUG(expr)                                                       \
    do {                                                                       \
    } while (0)
#endif

#define SCREEN_COORDINATE_COMPONENT_TYPE int32_t

/*
--------GRID--------------
*/

#define GRID_SIZE 8
#define DOUBLE_GRID_SIZE (8 * 2)

/*
---------------------------
*/

/*
---------PLAYER-----------
*/
// margin that player can overlap collidable objects in pixels
#define PLAYER_SIZE 16
#define PLAYER_COLLISION_BUFFER 2

/*
---------------------------
*/

/*
---------BLOCK-----------
*/
// number of frames to move a block one pixel
#define BLOCK_FRAMES_PER_MOVE 2

// are blocks walkable when loaded from the map
#define BLOCK_WALKABLE_ON_CREATE false

// width and height of block in pixels
#define BLOCK_SIZE 16

// margin between block bounding box and edge of sprite
#define BLOCK_BOUNDING_BOX_BUFFER 0

// block push animation distance in grid coords
#define BLOCK_PUSH_DISTANCE 2

#define BLOCK_PLAYER_SNAP_DIST                                                 \
    (BLOCK_SIZE - BLOCK_BOUNDING_BOX_BUFFER - PLAYER_COLLISION_BUFFER)

/*
---------------------------
*/