#include "entities.h"
#include "config.h"
#include "utils.h"
#include <GL/glut.h>
/*
extern struct Flatty {
  enum {
    FLATTY_WAITING, // before the game actually starts
    FLATTY_ALIVE,   // flapping and reacting to input
    FLATTY_DYING,   // dying animation
    FLATTY_DEAD     // not being displayed
  } state;

  f32 x, y, velY; // x is constant
} flatty;
*/

void initFlatty() {
  flatty.state = FLATTY_ALIVE;
  flatty.x = 64;
  flatty.y = 200;
  flatty.velY = 0;
}

void updateFlatty() {
  switch (flatty.state) {
    case FLATTY_WAITING:
      break;
    case FLATTY_ALIVE:
      // limit velocity to a maximum
      if (flatty.velY > -4) {
        flatty.velY -= 0.3f;
      }
      // only update if flatty still on screen
      if (flatty.y > 0) {
        flatty.y += flatty.velY;
      }
      break;
    case FLATTY_DYING:
      if (flatty.velY > -4) {
        flatty.velY -= 0.3f;
      }

      if (flatty.y > 0) {
        flatty.y += flatty.velY;
      } else { // out of screen
        flatty.state = FLATTY_DEAD;
      }
      break;
    case FLATTY_DEAD:
      break;
  }
}

void drawFlatty() {
  glPushMatrix();
    // body at (x, y)
    glTranslatef(flatty.x + 8, flatty.y + 5, 0);
    glRotatef(15*flatty.velY, 0, 0, 1); // part of flapping effect
    glScalef(16, 10, 1);
    glHexColor(0xd4bf26);
    glOval(0.5f);

    // eye
  glPopMatrix();
}

struct Flatty flatty;
