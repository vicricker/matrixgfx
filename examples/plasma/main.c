#include "plasma.h"
#include <led-matrix-c.h>
#include <matrixgfx.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

struct SetPoint rainbowSetPoints[] = {
    {.percent = 0, .color = {0xff, 0, 0}},
    {.percent = .16, .color = {0xff, 0x80, 0}},
    {.percent = .32, .color = {0xff, 0xff, 0}},
    {.percent = .48, .color = {0, 0xff, 0}},
    {.percent = .64, .color = {0, 0, 0xff}},
    {.percent = .80, .color = {0xff, 0, 0xff}},
    {.percent = .96, .color = {0x80, 0, 0xff}},
    {.percent = 1, .color = {0x80, 0, 0xff}}};

struct RGBLedMatrix *matrix;
struct LedCanvas *canvas;

struct Plasma *plasma;

uint8_t *bitmap;

struct IndexedImage image;

#define NCOLORS 256

struct Color palette[NCOLORS];

void exitHandler() {
  printf("\nExiting...\n");
  led_canvas_clear(canvas);

  led_matrix_delete(matrix);

  freePlasma(plasma);
  free(bitmap);
}

void sigHandler(const int s) {
  printf("\nKilled: %d\n", s);
  exit(0);
}

void createPalette() {
  struct Gradient gradient;

  gradient.nsetPoints = sizeof(rainbowSetPoints) / sizeof(struct SetPoint);
  gradient.setPoints = rainbowSetPoints;

  for (int i = 0; i < NCOLORS; i++) {
    gradientGetColor(&gradient, (float)i / (NCOLORS - 1), &palette[i]);
  }
}

int main(int argc, char **argv) {
  struct RGBLedMatrixOptions options;
  int width, height;

  atexit(exitHandler);
  signal(SIGINT, sigHandler);

  srand(clock());

  memset(&options, 0, sizeof(options));
  options.chain_length = 1;

  matrix = led_matrix_create_from_options(&options, &argc, &argv);
  if (matrix == NULL)
    return 1;

  canvas = led_matrix_create_offscreen_canvas(matrix);
  led_canvas_get_size(canvas, &width, &height);

  plasma = createPlasma(width, height);
  bitmap = malloc(width * height);
  image.bitmap = bitmap;
  image.width = width;
  image.height = height;

  createPalette();
  initTimer(&plasma->animationTimer, 20);

  while (1) {
    if (animationReady(&plasma->animationTimer)) {
      add_cosine(plasma, bitmap);

      blitIndexedImage(canvas, &image, palette, 0, 0);

      canvas = led_matrix_swap_on_vsync(matrix, canvas);
    }
  }

  return 0;
}
