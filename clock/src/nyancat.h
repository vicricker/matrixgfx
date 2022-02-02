#ifndef NYANCAT_H
#define NYANCAT_H

#include <led-matrix-c.h>
#include <matrixgfx.h>


int nyanCatX;
extern struct Image *nyanCat;
struct AnimationTimer nyanCatTimer;

void initNyanCat();

int advanceNyanCat();

void drawNyanCat(struct LedCanvas *canvas);

#endif
