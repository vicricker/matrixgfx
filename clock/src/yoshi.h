#ifndef YOSHI_H
#define YOSHI_H

#include <led-matrix-c.h>
#include <matrixgfx.h>

int yoshiX;
extern struct Image *yoshi;
struct AnimationTimer yoshiTimer;

void initYoshi();

int advanceYoshi();

void drawYoshi(struct LedCanvas *canvas);

#endif
