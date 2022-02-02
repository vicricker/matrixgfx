#ifndef SONIC_H
#define SONIC_H

#include <led-matrix-c.h>
#include <matrixgfx.h>

int sonicX;
extern struct Image *sonic;
struct AnimationTimer sonicTimer;

void initSonic();

int advanceSonic();

void drawSonic(struct LedCanvas *canvas);

#endif
