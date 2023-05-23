#ifndef ENTITIES_H
#define ENTITIES_H

#include "types.h"

extern struct Flatty {
  enum {
    FLATTY_WAITING, // before the game actually starts
    FLATTY_ALIVE,   // flapping and reacting to input
    FLATTY_DYING,   // dying animation
    FLATTY_DEAD     // not being displayed
  } state;

  f32 x, y, velY; // velX is constant
} flatty;

void initFlatty();
void updateFlatty();
void drawFlatty();

#define PIPE_WIDTH  32
#define PIPEPAIR_AMOUNT 6
#define PIPEPAIR_PASSAGE 60
#define PIPEPAIR_DISTANCE 64

#define COIN_SIZE 8

extern struct PipePair {
  f32 x;
  f32 passageY;

  enum {
    PIPEPAIR_NORMAL = 0,
    PIPEPAIR_WITH_COIN,
    PIPEPAIR_CRUSHER,
    PIPEPAIR_CRUSHER_WITH_COIN
  } type;

  struct Coin {
    i32 value;
    f32 angle;
  } coin;

  struct Crusher {
    enum {
      PIPEPAIR_CRUSHER_WAITING = 0, // waiting for activation
      PIPEPAIR_CRUSHER_ACTIVATED,   // transition to crushing state

      PIPEPAIR_CRUSHER_CRUSHING,    // object crushed, waiting to reset
      PIPEPAIR_CRUSHER_RESETING,    // transition to original waiting state
    } state;
    f32 passageOffset;
    i32 timer;
  } crusher;
} pipePairs[PIPEPAIR_AMOUNT];

bool collideWithPipePair(f32 x, f32 y, f32 w, f32 h);
bool collideWithPipePairCoin(f32 x, f32 y, f32 w, f32 h);

void initPipePairs();
void updatePipePairs();
void drawPipePairs();

#endif // ENTITIES_H

