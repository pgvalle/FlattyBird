#include "utils.h"
#include <GL/glut.h>
#include <math.h>

#define PI 3.14159265359f

// argb format
void glHexColor(u32 color) {
  //const i32 a = 0xff & (color >> 24);
  const i32 r = 0xff & (color >> 16);
  const i32 g = 0xff & (color >>  8);
  const i32 b = 0xff &  color;

  glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);
}

void glOval(f32 radius) {
  const i32 numIterations = 36;
  const f32 angleStep = 2*PI / numIterations; // in radians
  const f32 cosStep = cos(angleStep);
  const f32 sinStep = sin(angleStep);

  glBegin(GL_POLYGON);
    f32 cosi = 1;
    f32 sini = 0;
    for (i32 i = 0; i < numIterations; i++) {
      glVertex3f(radius*cosi, radius*sini, 0);
      
      const f32 cost = cosi;
      const f32 sint = sini;
      cosi = cost*cosStep - sint*sinStep; // cos(t=i + angleStep)
      sini = sint*cosStep + cost*sinStep; // sin(t=i + angleStep)
    }
  glEnd();
}
