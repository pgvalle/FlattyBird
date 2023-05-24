#include <GL/freeglut.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "utils.h"
#include "entities.h"

static enum {
  SCREEN_QUIT = 0,
  SCREEN_PLAY,
  SCREEN_PAUSE
} screen;
static int score;

static struct {
  enum {
    PLAYSCREEN_PLAYING,
    PLAYSCREEN_OVER
  } state;
  bool goingForward;
} playScreen;

static struct {
  enum {
    PAUSESCREEN_NORMAL,
    PAUSESCREEN_RESUMING // show a countdown
  } state;
  i32 timer;
} pauseScreen;

/////////////////
// PLAY SCREEN //
/////////////////

void keyboardPlayScreen(unsigned char key) {
  // quit game. Doesn't matter the state
  if (key == 'q') {
    screen = SCREEN_QUIT;
    return;
  }

  switch (playScreen.state) {
    case PLAYSCREEN_PLAYING:
      // escape key pressed
      if (key == 27) {
        pauseScreen.state = PAUSESCREEN_NORMAL;
        screen = SCREEN_PAUSE;
      } else if (key == ' ') {
        if (flatty.state == FLATTY_WAITING) {
          flatty.state = FLATTY_ALIVE;
        }
        // flap
        if (flatty.state == FLATTY_ALIVE) {
          flatty.velY = 4;
        }
      }
      break;
    case PLAYSCREEN_OVER:
      if (key == 27) {
        playScreen.state = PLAYSCREEN_PLAYING;
        // reset game state
        score = 0;
        initFlatty();
        initPipePairs();
      }
      break;
  }
}

void updatePlayScreen() {
  // play death animation
  if (flatty.state == FLATTY_ALIVE) {
    if (collideWithPipePair(flatty.x, flatty.y, 16, 10)) {
      flatty.state = FLATTY_DYING;
      flatty.velY = 4;
    } else if (collideWithPipePairCoin(flatty.x, flatty.y, 16, 10)) {
      score += 10;
    }
  }

  updateFlatty();
  if (flatty.state == FLATTY_ALIVE) {
    updatePipePairs();
    if (playScreen.goingForward) {
      for (int i = 0; i < PIPEPAIR_AMOUNT; i++) {
        pipePairs[i].x -= 0.5f;
      }
    }
  }

  if (flatty.state == FLATTY_DEAD) {
    playScreen.state = PLAYSCREEN_OVER;
  }
}

void displayPlayScreen() {  
  drawPipePairs();
  drawFlatty();

  // floor
  glHexColor(0x608426);
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

  if (playScreen.state == PLAYSCREEN_OVER) {
    // make it feel more like the game is over
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0, 0, 0, 0.4);
    glRectf(0, 0, WIDTH, HEIGHT);
    glDisable(GL_BLEND);

    // also render text displaying game over
    glColor3f(1, 1, 1);
    glRasterPos2i(0.45f*WIDTH, 0.5f*HEIGHT);
    glutBitmapString(GLUT_BITMAP_9_BY_15, "game over");
  }

  char scoreFmt[10];
  sprintf(scoreFmt, "score: %d", score);
  glColor3f(1, 1, 1);
  glRasterPos2i(8, HEIGHT - 10);
  glutBitmapString(GLUT_BITMAP_9_BY_15, scoreFmt);
}

//////////////////
// PAUSE SCREEN //
//////////////////

void keyboardPauseScreen(unsigned char key) {
  // quit the game
  if (key == 'q') {
    screen = SCREEN_QUIT;
    return;
  }

  switch (pauseScreen.state) {
    case PAUSESCREEN_NORMAL:
      // escape key pressed
      if (key == 27) {
        pauseScreen.state = PAUSESCREEN_RESUMING;
        pauseScreen.timer = 2999; // 3 seconds until resume
      }
      break;
    case PAUSESCREEN_RESUMING:
      if (key == 27) {
        pauseScreen.state = PAUSESCREEN_NORMAL;
      }
      break;
  }
}

void updatePauseScreen() {
  if (pauseScreen.state == PAUSESCREEN_RESUMING) {
    pauseScreen.timer -= 16;
    // countdown over. Resume game
    if (pauseScreen.timer <= 0) {
      screen = SCREEN_PLAY;
    }
  }
}

void displayPauseScreen() {
  displayPlayScreen();

  // make it feel more like the game is paused
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0, 0, 0, 0.4);
  glRectf(0, 0, WIDTH, HEIGHT);
  glDisable(GL_BLEND);

  // render countdown
  if (pauseScreen.state == PAUSESCREEN_RESUMING) {
    char countdownFmt[3];
    sprintf(countdownFmt, "%d", pauseScreen.timer / 1000 + 1);

    glColor3f(1, 1, 1);
    glRasterPos2i(0.5f*WIDTH, 0.5f*HEIGHT);
    glutBitmapString(GLUT_BITMAP_9_BY_15, countdownFmt);
  }
}

////////////////////
// MAIN FUNCTIONS //
////////////////////

void display();

void keyboard(unsigned char key, int x, int y) {
  switch (screen) {
    case SCREEN_PLAY:
      keyboardPlayScreen(key);
      break;
    case SCREEN_PAUSE:
      keyboardPauseScreen(key);
      break;
  }

  display();
}

void special(int key, int x, int y) {
  if (key == GLUT_KEY_RIGHT) {
    playScreen.goingForward = true;
  }
}

void specialUp(int key, int x, int y) {
  if (key == GLUT_KEY_RIGHT) {
    playScreen.goingForward = false;
  }
}

void update(i32 _) {
  switch (screen) {
    case SCREEN_PLAY:
      updatePlayScreen();
      break;
    case SCREEN_PAUSE:
      updatePauseScreen();
      break;
    case SCREEN_QUIT:
      glutLeaveMainLoop();
      break;
  }
  
  glutPostRedisplay();
  glutTimerFunc(1000 / FPS, update, 0);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  switch (screen) {
    case SCREEN_PLAY:
      displayPlayScreen();
      break;
    case SCREEN_PAUSE:
      displayPauseScreen();
      break;
  }

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

/////////////////
// ENTRY POINT //
/////////////////

i32 main(i32 argc, char** args) {
  srand(time(NULL));

  glutInit(&argc, args);
  glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(SCALE*WIDTH, SCALE*HEIGHT);

  glutCreateWindow("FlattyBird");

  glutDisplayFunc(display);
  glutTimerFunc(1000 / FPS, update, 0);
  // keyboard callbacks
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutSpecialUpFunc(specialUp);

  glClearColor(0x71/255.0f, 0xc6/255.0f, 0xd0/255.0f, 1);
  
  // setup world coordinate system
  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, 0, HEIGHT, -50, 50);
  initPipePairs();
  initFlatty();
  
  screen = SCREEN_PAUSE;
  playScreen.state = PLAYSCREEN_PLAYING;
  playScreen.goingForward = false;

  glutMainLoop();

  return 0;
}
