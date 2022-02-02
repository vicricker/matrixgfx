#ifndef PLASMA_H
#define PLASMA_H

#include <stdint.h>
#include <stdlib.h>
#include <matrixgfx.h>

struct Plasma
{
    int width;
    int height;
    struct AnimationTimer animationTimer;
    uint8_t i0, i1, i2, i3;
    uint8_t *cos_tab;
};

struct Plasma *createPlasma(int width, int height);
void freePlasma(struct Plasma *plasma);

void make_cosine(uint8_t *arr, size_t size);
void add_cosine(struct Plasma *plasma, uint8_t *buffer);

#endif
