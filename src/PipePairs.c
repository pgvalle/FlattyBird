#include "entities.h"
#include "config.h"
#include "utils.h"

#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

bool collideWithPipePair(f32 x, f32 y, f32 w, f32 h)
{
  for (i32 i = 0; i < PIPEPAIR_AMOUNT; i++) {
    // pipe pair bounds
    const f32 rx = pipePairs[i].x; // x and w are the same
    // top pipe
    const f32 ty = pipePairs[i].passageY + PIPEPAIR_PASSAGE -\
                   0.5f*pipePairs[i].crusher.passageOffset;
    const f32 th = HEIGHT - ty;
    // bottom pipe
    const f32 by = 0;
    const f32 bh = pipePairs[i].passageY + 0.5f*pipePairs[i].crusher.passageOffset;

    // bound checking
    if (x < rx + PIPE_WIDTH && x + w > rx && y < ty + th && y + h > ty || // top pipe 
        x < rx + PIPE_WIDTH && x + w > rx && y < by + bh && y + h > by)   // bottom pipe
    {
      return true;
    }
  }

  return false;
}

bool collideWithPipePairCoin(f32 x, f32 y, f32 w, f32 h)
{
  for (i32 i = 0; i < PIPEPAIR_AMOUNT; i++) {
    // not a relevant type of pipe pair. Skip
    if (pipePairs[i].type != PIPEPAIR_WITH_COIN &&
        pipePairs[i].type != PIPEPAIR_CRUSHER_WITH_COIN)
    {
      continue;
    }

    // coin position (left-bottom)
    const f32 cx = pipePairs[i].x + 0.5f*(PIPE_WIDTH - COIN_SIZE);
    const f32 cy = pipePairs[i].passageY + 0.5f*PIPEPAIR_PASSAGE;
    
    // bound checking
    if (x < cx + COIN_SIZE && x + w > cx && y < cy + COIN_SIZE && y + h > cy) {
      return true;
    }
  }

  return false;
}

static inline
f32 genRandomPassageY() {
  return 40 + 20*(rand() % 5);
}

static inline
int genRandomType() {
  return abs(rand() % 4);
}

static inline
f32 genRandomCoinRotation() {
  return abs(rand() % 360);
}

static inline
i32 genRandomCrushTimer() {
  return abs(rand() % 1000) + 1;
}

void initPipePairs() {
  for (i32 i = 0; i < PIPEPAIR_AMOUNT; i++) {
    pipePairs[i] = (struct PipePair){
      .x = (PIPEPAIR_DISTANCE + PIPE_WIDTH)*i + WIDTH,
      .passageY = genRandomPassageY(),

      .type = genRandomType(),

      .coin = { .value = 0, .angle = genRandomCoinRotation() },

      .crusher = {
        .state = PIPEPAIR_CRUSHER_WAITING,
        .passageOffset = 0,
        .timer = genRandomCrushTimer()
      }
    };
  }
}

void updateCrusher(struct Crusher* crusher) {
  crusher->timer += 16;
  switch (crusher->state) {
    case PIPEPAIR_CRUSHER_WAITING:
      if (crusher->timer >= 1000) {
        crusher->state = PIPEPAIR_CRUSHER_ACTIVATED;
        crusher->timer = 0;
      }
      break;
    case PIPEPAIR_CRUSHER_ACTIVATED:
      crusher->passageOffset += 3;
      if (crusher->passageOffset >= PIPEPAIR_PASSAGE) {
        crusher->passageOffset = PIPEPAIR_PASSAGE;
        crusher->state = PIPEPAIR_CRUSHER_CRUSHING;
        crusher->timer = 0;
      }
      break;
    case PIPEPAIR_CRUSHER_CRUSHING:
      if (crusher->timer >= 500) {
        crusher->state = PIPEPAIR_CRUSHER_RESETING;
        crusher->timer = 0;
      }
      break;
    case PIPEPAIR_CRUSHER_RESETING:
      crusher->passageOffset -= 1.5f;
      if (crusher->passageOffset <= 0) {
        crusher->passageOffset = 0;
        crusher->state = PIPEPAIR_CRUSHER_WAITING;
        crusher->timer = 0;
      }
      break;
  }

}

void updatePipePairs() {
  for (i32 i = 0; i < PIPEPAIR_AMOUNT; i++) {
    pipePairs[i].x -= 0.5f; // update position

    // update coin
    if (pipePairs[i].type == PIPEPAIR_WITH_COIN ||
        pipePairs[i].type == PIPEPAIR_CRUSHER_WITH_COIN)
    {
      pipePairs[i].coin.angle += 2;
      if (pipePairs[i].coin.angle >= 360) {
        pipePairs[i].coin.angle -= 360;
      }
    }
    
    if (pipePairs[i].type == PIPEPAIR_CRUSHER ||
        pipePairs[i].type == PIPEPAIR_CRUSHER_WITH_COIN)
    {
      updateCrusher(&pipePairs[i].crusher);
    }
  }

  // first pipe went fully out of screen
  if (pipePairs[0].x + PIPE_WIDTH <= 0) {
    // move all pipes overlapping first one (first pipe discarted)
    memmove(pipePairs, pipePairs + 1, (PIPEPAIR_AMOUNT - 1) * sizeof(struct PipePair));
    // "push" new pipepair
    pipePairs[PIPEPAIR_AMOUNT - 1] = (struct PipePair){
      .x = pipePairs[PIPEPAIR_AMOUNT - 1].x + PIPEPAIR_DISTANCE + PIPE_WIDTH,
      .passageY = genRandomPassageY(),

      .type = genRandomType(),

      .coin = { .value = 0, .angle = genRandomCoinRotation() },

      .crusher = {
        .state = PIPEPAIR_CRUSHER_WAITING,
        .passageOffset = 0,
        .timer = genRandomCrushTimer()
      }
    };
  }
}

void drawPipePairs() {
  for (i32 i = 0; i < PIPEPAIR_AMOUNT; i++) {
    // render coin in the middle and behind pipes
    if (pipePairs[i].type == PIPEPAIR_WITH_COIN ||
        pipePairs[i].type == PIPEPAIR_CRUSHER_WITH_COIN)
    {
      glHexColor(0xffffff);
      // draw coin
      glPushMatrix();
        glTranslatef(pipePairs[i].x + 0.5f*PIPE_WIDTH,
            pipePairs[i].passageY + 0.5f*PIPEPAIR_PASSAGE, 0);
        glRotatef(pipePairs[i].coin.angle, 0, 1, 0);
        glScalef(COIN_SIZE, COIN_SIZE, 1);
        glutSolidSphere(0.5f, 24, 24);
      glPopMatrix();
    }

    glHexColor(0x75be2f); // green
    // top pipe
    glPushMatrix(); {
      const f32 pipeHeight = HEIGHT - pipePairs[i].passageY - PIPEPAIR_PASSAGE +\
                             0.5f*pipePairs[i].crusher.passageOffset;
      glTranslatef(pipePairs[i].x + 0.5f*PIPE_WIDTH, HEIGHT - 0.5f*pipeHeight, 0);
      glScalef(PIPE_WIDTH, pipeHeight, PIPE_WIDTH);
      glutSolidCube(1); }
    glPopMatrix();
    // bottom pipe
    glPushMatrix(); {
      const f32 pipeHeight = pipePairs[i].passageY + 0.5f*pipePairs[i].crusher.passageOffset;
      glTranslatef(pipePairs[i].x + 0.5f*PIPE_WIDTH, 0.5f*pipeHeight, 0);
      glScalef(PIPE_WIDTH, pipeHeight, PIPE_WIDTH);
      glutSolidCube(1); }
    glPopMatrix();
  }
}

struct PipePair pipePairs[PIPEPAIR_AMOUNT];
