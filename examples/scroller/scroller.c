#include <led-matrix-c.h>
#include <matrixgfx.h>
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

struct SetPoint rainbowSetPoints[] = {
    {.percent = 0, .color = {0xff, 0, 0}},
    {.percent = .16, .color = {0xff, 0x80, 0}},
    {.percent = .32, .color = {0xff, 0xff, 0}},
    {.percent = .48, .color = {0, 0xff, 0}},
    {.percent = .64, .color = {0, 0, 0xff}},
    {.percent = .80, .color = {0xff, 0, 0xff}},
    {.percent = .96, .color = {0x80, 0, 0xff}},
    {.percent = 1, .color = {0x80, 0, 0xff}}};

struct Scroller *scroller;

struct Gradient rainbowGradient = {0};

struct RGBLedMatrix *matrix;
struct LedCanvas *canvas;
struct LedFont *font;
struct LedFont *outlineFont;

void exitHandler() {
  printf("\nExiting...\n");
  led_canvas_clear(canvas);

  led_matrix_delete(matrix);

  delete_font(font);
  delete_font(outlineFont);

  freeScroller(scroller);
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

  font = load_font("fonts/10x20.bdf");
  outlineFont = create_outline_font(font);

  struct LedFont *fonts[] = {font, outlineFont};

  rainbowGradient.nsetPoints =
      sizeof(rainbowSetPoints) / sizeof(struct SetPoint);
  rainbowGradient.setPoints = rainbowSetPoints;

  struct Gradient gradient = {0};
  gradient.nsetPoints = sizeof(fireSetPoints) / sizeof(struct SetPoint);
  gradient.setPoints = fireSetPoints;

  struct Gradient *gradients[] = {&gradient, &rainbowGradient};

  // clang-format destroys this.  Sorry.
  char text[] = "        Scroller supports combinations of: "
    FONT(1) "Fonts, " FONT(0)
    COLOR(F00000) "C" COLOR(00F000) "O" COLOR(0000F0) "L" COLOR(F0F000) "O" COLOR(F08000) "R" COLOR(F00080) "S"
    COLOR(808080) ", " GRADIENT(1) "Gradients" GRADIENT(2) ", and " SINE(4) " Sine Waves " SINE(0);

  scroller = createScroller(0, width, fonts, sizeof(fonts)/sizeof(fonts[0]), gradients, sizeof(gradients)/sizeof(gradients[0]), text);
  scroller->y = 26;
  initTimer(&scroller->animationTimer, 10);

  while (1) {
    led_canvas_clear(canvas);

    drawScroller(canvas, scroller);
    advanceScroller(scroller);

    canvas = led_matrix_swap_on_vsync(matrix, canvas);
  }
}
