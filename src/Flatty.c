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
  flatty.state = FLATTY_WAITING;
  flatty.x = 64;
  flatty.y = 122.5;
  flatty.velY = 0;
}

void updateFlatty() {
  switch (flatty.state) {
    case FLATTY_ALIVE:
      // limit velocity to a maximum
      if (flatty.velY > -4) {
        flatty.velY -= 0.3f;
      }
      // only update if flatty still on screen
      if (flatty.y > 0) {
        flatty.y += flatty.velY;
      } else { // out of screen: dead
        flatty.state = FLATTY_DEAD;
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
  }
}

void drawFlatty() {
  glPushMatrix();
    // body
    glHexColor(0xd4bf26);
    glTranslatef(flatty.x + 8, flatty.y + 5, 0);
    glRotatef(15*flatty.velY, 0, 0, 1); // body flapping effect
    glPushMatrix();
      glScalef(16, 10, 1);
      glOval(0.5f);
    glPopMatrix();

    // handle
    glHexColor(0xf3f4e3);
    glRotatef(-90 + 15*flatty.velY, 1, 0, 0); // wing flapping effect
    glTranslatef(-2, -2, 0);
    glScalef(8, 4, 1);
    glBegin(GL_POLYGON);
      glVertex2f(-0.5f, -0.5f);
      glVertex2f(0.5f, 0.5f);
      glVertex2f(-0.5f, 0.5f);
    glEnd();
  glPopMatrix();
}

struct Flatty flatty;
