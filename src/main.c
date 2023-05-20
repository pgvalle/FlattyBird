#define STB_DS_IMPLEMENTATION
#include "../ext/stb_ds.h"

#include <GL/glut.h>
#include "config.h"
#include "utils.h"
#include "entities.h"

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  drawPipePairs();

  glFlush();
}

void reshape(i32 w, i32 h) {
  glViewport(0, 0, w, h);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  display();
}

void timer(i32 _) {
    updatePipePairs();

    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, 0);
}

i32 main(i32 argc, char** args) {
  glutInit(&argc, args);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(SCALE*WIDTH, SCALE*HEIGHT);

  glutCreateWindow("FlattyBird");

  glutDisplayFunc(display);
  glutTimerFunc(1000 / FPS, timer, 0);

  glClearColor(0x71/255.0f, 0xc6/255.0f, 0xd0/255.0f, 1);
  
  // setup world coordinate system
  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, 0, HEIGHT, -1.0, 1.0);
  initPipePairs();

  glutMainLoop();

  return 0;
}
