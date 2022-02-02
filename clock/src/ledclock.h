#ifndef LEDCLOCK_H
#define LEDCLOCK_H
#include <led-matrix-c.h>
#include <matrixgfx.h>

struct LedClock
{
    struct LedFont *font;

    struct AnimationTimer animationTimer;

    char text[9];
};

struct LedClock *createLedClock(struct LedFont *font);

void advanceLedClock( struct LedClock *clock);

void drawLedClock(struct LedCanvas *canvas, struct LedClock *clock);

#endif
