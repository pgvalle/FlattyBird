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

  drawFlatty();
  drawPipePairs();

  glFlush();
}

void reshape(i32 w, i32 h) {
  glViewport(0, 0, w, h);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  display();
}

static inline
void toGameCoords(f32* x, f32* y, f32 sx, f32 sy) {
  *x = WIDTH * sx / glutGet(GLUT_WINDOW_WIDTH);
  const f32 sh = glutGet(GLUT_WINDOW_HEIGHT);
  *y = HEIGHT * (sh - sy) / sh;
}

void timer(i32 _) {
    updateFlatty();
    updatePipePairs();

    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, 0);
}

void mouse(int button, int state, int x, int y) {
  f32 fx, fy;
  toGameCoords(&fx, &fy, x, y);
  if (collideWithPipePairCoin(fx, fy, 1, 1)) {
    // do some sht
  }
}

i32 main(i32 argc, char** args) {
  srand(time(NULL));

  glutInit(&argc, args);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(SCALE*WIDTH, SCALE*HEIGHT);

  glutCreateWindow("FlattyBird");

  glutDisplayFunc(display);
  glutTimerFunc(1000 / FPS, timer, 0);
  glutMouseFunc(mouse);

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
