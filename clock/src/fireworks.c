#include <led-matrix-c.h>
#include <matrixgfx.h>
#include <stdlib.h>
#include <time.h>
#include "fireworks.h"

struct Color colors[] = {{0xff, 0, 0},    {0xff, 0x80, 0},   {0xff, 0xff, 0},
                         {0, 0xff, 0},    {0, 0, 0xff},      {0, 0x80, 0xff},
                         {0, 0xff, 0xff}, {0xff, 0xff, 0xff}

};

struct AnimationTimer fireworksTimer;

#define NCOLORS (sizeof(colors) / sizeof(struct Color))

int colorIndex = 0;

struct Firework fireworks[MAX_FIREWORKS];

int randomRange(int low, int high) { return low + rand() % (high - low); }

void resetFirework(struct Firework *firework) {
  firework->x = rand() % 100 * 128 / 100;
  firework->y = 10 + rand() % 12;
  firework->age = 0;

  struct Color *c = &colors[colorIndex % NCOLORS];
  colorIndex++;

  for (int s = 0; s < MAX_SPARKS; s++) {
    struct Spark *spark = &firework->sparks[s];

    spark->x = firework->x;
    spark->y = firework->y;

    spark->vx = (double)randomRange(1, 3) + .5;
    spark->vy = (double)randomRange(1, 3) + .5;

    spark->weight = (double)(rand() % 100) * .4 / 100 + .03;

    if (rand() & 1)
      spark->vx = -spark->vx;
    if (rand() & 1)
      spark->vy = -spark->vy;

    spark->color.r = c->r;
    spark->color.g = c->g;
    spark->color.b = c->b;
  }
}

struct Color *randColor() {
  return &colors[rand() % sizeof(colors) / sizeof(struct Color)];
}

void initFireworks() {
  srand(time(0));

  initTimer(&fireworksTimer, 10);

  for (int i = 0; i < MAX_FIREWORKS; i++) {
    struct Firework *firework = &fireworks[i];
    resetFirework(firework);
  }
}

void advanceFireworks() {
  if (animationReady(&fireworksTimer)) {

    for (int f = 0; f < MAX_FIREWORKS; f++) {
      struct Firework *firework = &fireworks[f];

      firework->age++;
      if (firework->age > 10 && rand() % 100 < 5) {
        resetFirework(firework);
      }
    }
  }
}

void drawFireworks(struct LedCanvas *canvas) {
  for (int f = 0; f < MAX_FIREWORKS; f++) {
    struct Firework *firework = &fireworks[f];

    for (int s = 0; s < MAX_SPARKS; s++) {
      struct Spark *spark = &firework->sparks[s];

      for (int i = 0; i < 10; i++) {
        int trailAge = firework->age + i;
        int x = firework->x + spark->vx * trailAge;
        int y = (double)firework->y + spark->vy * trailAge +
                spark->weight * trailAge * spark->weight * trailAge;

        double fade = i * 20 - firework->age * 10;

        int r = (int)(spark->color.r * fade) & 255;
        int g = (int)(spark->color.g * fade) & 255;
        int b = (int)(spark->color.b * fade) & 255;

        led_canvas_set_pixel(canvas, x, y % 128, r, g, b);
      }
    }
  }
}
