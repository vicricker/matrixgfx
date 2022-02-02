#include <led-matrix-c.h>
#include <matrixgfx.h>
#include "sonic.h"


int sonicX;
struct Image *sonic;
struct AnimationTimer sonicTimer;

void initSonic() {
  sonicX = 0;
  initTimer(&sonicTimer, 10);
}

int advanceSonic() {
  int sonicRunning = 1;

  if (animationReady(&sonicTimer)) {
    if (sonicX++ >= 128) {
      sonicRunning = 0;
    }
  }

  return sonicRunning;
}

void drawSonic(struct LedCanvas *canvas) {
  blit(canvas, sonic, sonicX / 10 % 3, sonicX, 0);
}
