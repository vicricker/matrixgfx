#ifndef PARTICLE_H
#define PARTICLE_H

#include <led-matrix-c.h>
#include <matrixgfx.h>

#define MAX_FIREWORKS 4
#define MAX_SPARKS 30

struct Spark {
  int x, y;
  double vx, vy;
  double weight;
  struct Color color;
};

struct Firework {
  int x;
  int y;
  int age;

  struct Spark sparks[MAX_SPARKS];
};

void drawFireworks(struct LedCanvas *canvas);
void advanceFireworks();
void initFireworks();
#endif
