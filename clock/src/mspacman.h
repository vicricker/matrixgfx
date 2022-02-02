#ifndef MSPACMAN_H
#define MSPACMAN_H

#include <led-matrix-c.h>
#include <matrixgfx.h>

int msPacManX;
extern struct Image *msPacMan;
extern struct Image *sue;
struct AnimationTimer msPacManTimer;

void initMsPacMan();

int advanceMsPacMan();

void drawMsPacMan(struct LedCanvas *canvas);
#endif