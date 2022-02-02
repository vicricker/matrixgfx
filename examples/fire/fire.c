#include "fire.h"
#include <led-matrix-c.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

struct Color fireColors[] = {
    {0, 0, 0}, {0xff, 0, 0}, {0xff, 0x80, 0}, {0xff, 0xff, 0}, {0, 0, 0}};

struct SetPoint fireSetPoints[] = {{.percent = 0, .color = {0, 0, 0}},
                                   {.percent = .1, .color = {0xff, 0, 0}},
                                   {.percent = .2, .color = {0xff, 0x80, 0}},
                                   {.percent = .3, .color = {0xff, 0xff, 0}},
                                   {.percent = 1, .color = {0xff, 0xff, 0xff}}};

struct Fire *fire;

struct Fire *createFire(int width, int height) {
  struct Fire *fire = calloc(1, sizeof(struct Fire));

  initTimer(&fire->animationTimer, 20);
  fire->width = width;
  fire->height = height;

  fire->gray = malloc(fire->width * fire->height +
                      fire->width); // One extra line to hide the seed row

  fire->gradient.nsetPoints = sizeof(fireSetPoints) / sizeof(struct SetPoint);
  fire->gradient.setPoints = fireSetPoints;

  return fire;
}

void freeFire(struct Fire *fire) {
  free(fire->gray);
  free(fire);
}

void seed(struct Fire *fire) {
  uint8_t *bottom = fire->gray + fire->width * fire->height;
  memset(bottom, 0, fire->width);

  for (int i = 0; i < 7; i++) {
    int spot = rand() % fire->width;

    for (int j = 0; j < 5; j++) {
      int offset = rand() % 10 - 5;

      int x = spot + offset;

      x = x % fire->width;

      bottom[x] = (100 + rand() % 155) & 0xff;
    }
  }
}

void average(struct Fire *fire) {
  for (int y = 0; y < fire->height; y++) {
    uint8_t *row1 = fire->gray + fire->width * y + 1;
    uint8_t *row2 = row1 + fire->width;

    for (int x = 0; x < fire->width - 2; x++) {
      int p = (row1[x] + row2[x - 1] + row2[x + 1]) / 3;

      if (p) {
        p--;
      }
      row1[x] = p;
    }
  }
}

void advanceFire(struct Fire *fire) {
  if (animationReady(&fire->animationTimer)) {
    seed(fire);
    average(fire);
  }
}

void drawFire(struct LedCanvas *canvas, struct Fire *fire) {
  uint8_t *p = fire->gray;

  for (int y = 0; y < fire->height; y++) {
    for (int x = 0; x < fire->width; x++) {
      uint8_t c = *p++;

      struct Color color;
      gradientGetColor(&fire->gradient, (float)c / 255, &color);

      led_canvas_set_pixel(canvas, x, y, color.r, color.g, color.b);
    }
  }
}

struct RGBLedMatrix *matrix;
struct LedCanvas *canvas;

void exitHandler() {
  printf("\nExiting...\n");
  led_canvas_clear(canvas);

  led_matrix_delete(matrix);

  free(fire);
}

void sigHandler(const int s) {
  printf("\nKilled: %d\n", s);
  exit(0);
}

int main(int argc, char **argv) {
  struct RGBLedMatrixOptions options;
  int width, height;

  atexit(exitHandler);
  signal(SIGINT, sigHandler);

  srand(clock());

  memset(&options, 0, sizeof(options));
  options.rows = 32;
  options.chain_length = 1;

  matrix = led_matrix_create_from_options(&options, &argc, &argv);
  if (matrix == NULL)
    return 1;

  canvas = led_matrix_create_offscreen_canvas(matrix);
  led_canvas_get_size(canvas, &width, &height);

  struct Fire *fire = createFire(width, height);

  while (1) {
    led_canvas_clear(canvas);
    advanceFire(fire);

    drawFire(canvas, fire);

    canvas = led_matrix_swap_on_vsync(matrix, canvas);
  }
}
