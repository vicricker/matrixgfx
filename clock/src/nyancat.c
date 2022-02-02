#include <led-matrix-c.h>
#include <matrixgfx.h>
#include "nyancat.h"

int nyanCatX;
struct Image *nyanCat;
struct AnimationTimer nyanCatTimer;

void initNyanCat() {
  nyanCatX = -150;
  initTimer(&nyanCatTimer, 20);
}

int advanceNyanCat() {
  int running = 1;

  if (animationReady(&nyanCatTimer)) {
    if (nyanCatX++ >= 128) {
      running = 0;
    }
  }

  return running;
}

void drawNyanCat(struct LedCanvas *canvas) {
  blit(canvas, nyanCat, nyanCatX/4, nyanCatX, 6);
}
