#ifndef FIRE_H
#define FIRE_H

#include <led-matrix-c.h>
#include <matrixgfx.h>
#include <stdint.h>

struct Fire {
  int width;
  int height;
  struct AnimationTimer animationTimer;
  uint8_t *gray;
  struct Gradient gradient;
};

struct Fire *createFire(int width, int height);
void freeFire(struct Fire *fire);
void advanceFire(struct Fire *fire);
void drawFire(struct LedCanvas *canvas, struct Fire *fire);

#endif
