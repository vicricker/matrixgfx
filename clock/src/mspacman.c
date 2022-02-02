#include <led-matrix-c.h>
#include <matrixgfx.h>
#include "mspacman.h"

int msPacManX;
struct Image *msPacMan;
struct Image *sue;
struct AnimationTimer msPacManTimer;

void initMsPacMan() {
  msPacManX = -32;
  initTimer(&msPacManTimer, 10);
}

int advanceMsPacMan() {
  int running = 1;

  if (animationReady(&msPacManTimer)) {
    if (msPacManX++ >= 128 + 40) {
      running = 0;
    }
  }

  return running;
}

void drawMsPacMan(struct LedCanvas *canvas) {
  blit(canvas, msPacMan, msPacManX / 10 % 3, msPacManX, 0);
  blit(canvas, sue, msPacManX / 10 % 3, msPacManX - 40, 0);
}
