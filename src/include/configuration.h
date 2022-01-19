#pragma once

//-------------------------//
// GAME CONFIGURATION FLAGS//
//-------------------------//
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