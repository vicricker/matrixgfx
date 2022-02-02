#include <led-matrix-c.h>
#include <matrixgfx.h>
#include "yoshi.h"

int yoshiX;
struct Image *yoshi;
struct AnimationTimer yoshiTimer;

void initYoshi() {
  yoshiX = -32;
  initTimer(&yoshiTimer, 20);
}

int advanceYoshi() {
  int running = 1;

  if (animationReady(&yoshiTimer)) {
    if (yoshiX++ >= 128) {
      running = 0;
    }
  }

  return running;
}

void drawYoshi(struct LedCanvas *canvas) {
  blit(canvas, yoshi, yoshiX / 10 % 3, yoshiX, 0);
}
