#include <GL/glut.h>
#include "config.h"
#include "utils.h"

// palette
//  sky: #71c6d0
//  clouds: 

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  glPushMatrix();
  glTranslatef(100, 100, 0);
  glScalef(100, 100, 1);
  glHexColor(0xffffff);
  glOval(0.5f, 0);
  glPopMatrix();

  glFlush();
}

void reshape(i32 w, i32 h) {
  glViewport(0, 0, w, h);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  display();
}

void timer(i32 _) {
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

  glutMainLoop();

  return 0;
}
