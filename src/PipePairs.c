#include "entities.h"
#include "config.h"
#include "utils.h"
#include <GL/glut.h>
#include <string.h>

/*extern struct PipePair {
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
}* pipePairs;*/

void initPipePairs() {
  for (int i = 0; i < PIPEPAIR_AMOUNT; i++) {
    // all pipes start out of screen
    pipePairs[i].x = WIDTH + (PIPEPAIR_DISTANCE + PIPE_WIDTH)*(i + 1);
    pipePairs[i].passageY = 100;
    pipePairs[i].crusher = false;
  }
}

void updatePipePairs() {
  for (int i = 0; i < PIPEPAIR_AMOUNT; i++) {
    pipePairs[i].x -= 1; // move backwards

    if (pipePairs[i].crusher) {

    }
  }

  // first pipe went fully out of screen
  if (pipePairs[0].x + PIPE_WIDTH <= 0) {
    struct PipePair newPipePair = {
      .x = pipePairs[PIPEPAIR_AMOUNT - 1].x + PIPEPAIR_DISTANCE + PIPE_WIDTH,
      .passageY = 100,
      .crusher = false,
      .crusherState = PIPEPAIR_CRUSH_WAIT,
      .crusherTimer = 0
    };

    const size_t typeSize = sizeof(struct PipePair);
    // move all pipes overlapping first one (first pipe discarted)
    memmove(&pipePairs[0], &pipePairs[1], (PIPEPAIR_AMOUNT - 1) * typeSize);
    // "push" new pipe
    memcpy(&pipePairs[PIPEPAIR_AMOUNT - 1], &newPipePair, typeSize);
  }
}

void drawPipePairs() {
  glHexColor(0x75be2f);
  for (int i = 0; i < PIPEPAIR_AMOUNT; i++) {
    // bottom pipe
    glRectf(pipePairs[i].x, 0,
      pipePairs[i].x + PIPE_WIDTH, pipePairs[i].passageY);
    // top pipe
    glRectf(pipePairs[i].x, pipePairs[i].passageY + PIPEPAIR_PASSAGE_HEIGHT,
        pipePairs[i].x + PIPE_WIDTH, HEIGHT);
  }
}

struct PipePair pipePairs[PIPEPAIR_AMOUNT];
