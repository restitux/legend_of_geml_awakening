#pragma once

//-------------------------//
// GAME CONFIGURATION FLAGS//
//-------------------------//

// DEBUG
#define DEBUG_DRAWS

#ifdef DEBUG_DRAWS
#define ONLY_DEBUG(expr)                                                       \
  do {                                                                         \
    expr;                                                                      \
  } while (0)
#else
#define ONLY_DEBUG(expr)                                                       \
  do {                                                                         \
  } while (0)
#endif

/*
---------PLAYER-----------
*/
// margin that player can overlap collidable objects in pixels
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

/*
---------------------------
*/