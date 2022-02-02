#ifndef MATRIXGFX_H
#define MATRIXGFX_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <led-matrix-c.h>

struct AnimationTimer {
  clock_t delayTicks;
  clock_t startTime;
};

struct Anim {
  void (*init)();
  int (*advance)();
  void (*draw)(struct LedCanvas *);
};

void initTimer(struct AnimationTimer *timer, clock_t delayMs);

int animationReady(struct AnimationTimer *timer);

struct Color {
  uint8_t r, g, b;
};

struct SetPoint {
  float percent;
  struct Color color;
};

struct Gradient {
  int nsetPoints;

  struct SetPoint *setPoints;
};

int findComponent(int s, int e, float percent);

void findColor(struct SetPoint *s, struct SetPoint *e, float percent,
               struct Color *color);

void gradientGetColor(struct Gradient *gradient, float percent,
                      struct Color *color);

struct RGBA {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

typedef union Pixel {
  struct RGBA rgba;
  uint32_t val;
} Pixel;

struct Image {
  int width;
  int height;

  int imageCount;

  Pixel *bitmaps;
};

struct IndexedImage {
  int width;
  int height;

  uint8_t *bitmap;
};

Pixel *getFrame(struct Image *, unsigned int frame);

void freeImage(struct Image *);
void freeIndexedImage(struct IndexedImage *);

void blit(struct LedCanvas *canvas, struct Image *image, unsigned int frame,
          int x, int y);
void blitFlipHoriz(struct LedCanvas *canvas, struct Image *image,
                   unsigned int frame, int x, int y);
void bitFlip(struct LedCanvas *canvas, struct Image *image, unsigned int frame,
             int x, int y, int flip);

void blitIndexedImage(struct LedCanvas *canvas, struct IndexedImage *image,
                      struct Color *palette, int x, int y);

void freeIndexedImage(struct IndexedImage *image);

#define ESC 0x1b

struct Attributes {
  int fontIndex;
  int sine;
  struct Color color;
  int gradient;
};

struct Scroller {
  struct AnimationTimer animationTimer;
  int x,y;
  unsigned int width;
  char *message;
  int offset;
  int currentWidth;
  int currentPos;
  unsigned int nfonts;
  struct LedFont **fonts;
  struct Attributes **attributes;
  unsigned int ngradients;
  struct Gradient **gradients;
};

#define FONT(font) "\033F" #font
#define COLOR(rrggbb) "\033C" #rrggbb
#define SINE(freq) "\033S" #freq
#define GRADIENT(gradient) "\033G" #gradient

struct Scroller *createScroller(unsigned int x, unsigned int width, struct LedFont **font, int nfonts,struct Gradient **gradients, int ngradients, char *msg);
void drawScroller(struct LedCanvas *canvas, struct Scroller *scroller);
void advanceScroller(struct Scroller *scroller);
void freeScroller(struct Scroller *scroller);

struct Image *loadJPEG(char *filename);

struct Image *loadGIF(char *filename);

#endif
