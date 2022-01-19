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

// Block

#define FRAMES_PER_MOVE 2
#define BLOCK_WALKABLE_ON_CREATE false

#define BLOCK_RAISED_SPRITE block_sprite
#define BLOCK_LOWERED_SPRITE
#define BLOCK_SIZE 16