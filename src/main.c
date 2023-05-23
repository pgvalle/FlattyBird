#define STB_DS_IMPLEMENTATION
#include "../ext/stb_ds.h"

#include <GL/glut.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "utils.h"
#include "entities.h"

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  
  drawPipePairs();
  drawFlatty();

  // floor
  glHexColor(0x0608426);
  glPushMatrix();
    glTranslatef(0.5f*WIDTH, 20 + 2.5f, 0);
    glScalef(WIDTH, 5, 20);
    glutSolidCube(1);
  glPopMatrix();
  // below floor
  glHexColor(0xfcc589);
  glPushMatrix();
    glTranslatef(0.5f*WIDTH, 10, 0);
    glScalef(WIDTH, 20, 20);
    glutSolidCube(1);
  glPopMatrix();

  glFlush();
}

void reshape(i32 w, i32 h) {
  glViewport(0, 0, w, h);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  display();
}

static inline
void toWorldCoords(f32* x, f32* y, f32 sx, f32 sy) {
  *x = WIDTH * sx / glutGet(GLUT_WINDOW_WIDTH);
  const f32 sh = glutGet(GLUT_WINDOW_HEIGHT);
  *y = HEIGHT * (sh - sy) / sh;
}

bool goingLeft = false;

void timer(i32 _) {
    if (flatty.state == FLATTY_ALIVE && collideWithPipePair(flatty.x, flatty.y, 16, 10)) {
      flatty.state = FLATTY_DYING;
      flatty.velY = 4;
    }

    if (goingLeft) {
      for (int i = 0; i < PIPEPAIR_AMOUNT; i++) {
        pipePairs[i].x -= 0.5f;
      }
    }

    updateFlatty();
    updatePipePairs();

    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
  if (key == ' ' && flatty.state < 2) {
    flatty.velY = 4;
  }

  if (key == 'e') {
    goingLeft = true;
  }

  display();
}

void keyboardUp(unsigned char key, int x, int y) {
  if (key == 'e') {
    goingLeft = false;
  }
  display();
}

void mouse(int button, int state, int x, int y) {
  f32 fx, fy;
  toWorldCoords(&fx, &fy, x, y);
  if (collideWithPipePair(fx, fy, 1, 1)) {
    printf("Helo ma fren\n");
  }
}

i32 main(i32 argc, char** args) {
  srand(time(NULL));

  glutInit(&argc, args);
  glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(SCALE*WIDTH, SCALE*HEIGHT);

  glutCreateWindow("FlattyBird");

  glutDisplayFunc(display);
  glutTimerFunc(1000 / FPS, timer, 0);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyboardUp);

  glClearColor(0x71/255.0f, 0xc6/255.0f, 0xd0/255.0f, 1);
  
  // setup world coordinate system
  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, 0, HEIGHT, -50, 50);
  initPipePairs();
  initFlatty();

  glutMainLoop();

  return 0;
}
