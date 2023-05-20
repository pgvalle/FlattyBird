#ifndef ENTITIES_H
#define ENTITIES_H

#include "types.h"

/*extern struct Flatty {
  enum {
    FLATTY_WAITING, // before the game actually starts
    FLATTY_ALIVE,   // flapping and reacting to input
    FLATTY_DYING,   // dying animation
    FLATTY_DEAD     // not being displayed
  } state;

  f32 x, y; // x is constant
  f32 a; // acceleration (y axis only)
} flatty;

void updateFlatty();
void drawFlatty(); */

#define PIPE_WIDTH 32
#define PIPEPAIR_AMOUNT 10
#define PIPEPAIR_DISTANCE 48
#define PIPEPAIR_PASSAGE_HEIGHT 64 

extern struct PipePair {
  f32 x;
  f32 passageY;

  bool crusher;
  enum {
    PIPEPAIR_CRUSH_WAIT,
    PIPEPAIR_CRUSHING,
    PIPEPAIR_RESET_WAIT,
    PIPEPAIR_RESETTING
  } crusherState;
  u32 crusherTimer;
} pipePairs[PIPEPAIR_AMOUNT];

void initPipePairs();
void updatePipePairs();
void drawPipePairs();

#endif // ENTITIES_H

