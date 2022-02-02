#include "fireworks.h"
#include "ledclock.h"
#include "mspacman.h"
#include "nyancat.h"
#include "sonic.h"
#include "yoshi.h"
#include <led-matrix-c.h>
#include <matrixgfx.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct tm *now(void);

int debug = 0;

struct Anim spriteAnims[] = {
    {.init = initNyanCat, .advance = advanceNyanCat, .draw = drawNyanCat},
    {.init = initYoshi, .advance = advanceYoshi, .draw = drawYoshi},
    {.init = initSonic, .advance = advanceSonic, .draw = drawSonic},
    {.init = initMsPacMan, .advance = advanceMsPacMan, .draw = drawMsPacMan},
};

struct RGBLedMatrix *matrix;
struct LedCanvas *canvas;
struct LedFont *font;
struct LedFont *fonts[3];
struct LedFont *sevenSegmentFont;
struct LedFont *outlineFont;

enum State { IDLE, COUNTDOWN, NEWYEAR };

void exitHandler() {
  printf("Exiting...\n");
  led_canvas_clear(canvas);
  delete_font(font);
  delete_font(outlineFont);
  delete_font(sevenSegmentFont);

  freeImage(sonic);
  freeImage(msPacMan);
  freeImage(nyanCat);
  freeImage(sue);
  freeImage(yoshi);

  led_matrix_delete(matrix);
}

void sigHandler(const int s) {
  printf("Killed: %d\n", s);
  exit(0);
}

clock_t countdownDigitStart = 0;
int prevRemaining = 0;
int countdownElapsed = 0;

int advanceCountdown() {
  struct tm *timeinfo = now();

  int remainingSeconds = (60 - timeinfo->tm_sec) % 60;

  if (remainingSeconds != prevRemaining) {
    prevRemaining = remainingSeconds;
    countdownDigitStart = clock();
  }

  countdownElapsed = (clock() - countdownDigitStart) * 1000 / CLOCKS_PER_SEC;

  return remainingSeconds;
}

int targetHour;
int targetMinute;
int targetSecond;

struct tm *now() {
  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  return timeinfo;
}

int isTargetTime() {
  struct tm *timeinfo = now();

  return timeinfo->tm_hour == targetHour && timeinfo->tm_min == targetMinute &&
         timeinfo->tm_sec == targetSecond;
}

void drawCountdown(struct LedCanvas *canvas, int count) {
  char text[3];
  sprintf(text, "%d", count);

  int offset = strlen(text) * 5; // 5 is half the font width

  int color = ((1000 - countdownElapsed) * 255 / 1000) & 0xff;
  int pos = countdownElapsed * 64 / 1000;

  draw_text(canvas, font, 64 - offset - pos, 24, color, color, color, text, 0);

  draw_text(canvas, font, 64 - offset + pos, 24, color, color, color, text, 0);
}

int main(int argc, char **argv) {
  struct RGBLedMatrixOptions options;
  int width, height;

  srand(clock());

  for (int c = 1; c < argc; c++) {
    if (*argv[c] != '-') {
      debug = 1;
    }
  }

  if (debug) {
    struct tm *timeinfo = now();
    targetHour = timeinfo->tm_hour;
    targetMinute = timeinfo->tm_min;

    targetSecond = timeinfo->tm_sec + 10;
    if (targetSecond > 60) {
      targetSecond = targetSecond - 60;
      targetMinute++;
    }
  } else {
    targetHour = 23;
    targetMinute = 59;
    targetSecond = 50;
  }

  printf("Target time: %02d:%02d:%02d\n", targetHour, targetMinute,
         targetSecond);

  sonic = loadGIF("sprites/sonic.gif");
  msPacMan = loadGIF("sprites/mspacman.gif");
  nyanCat = loadGIF("sprites/nyancat.gif");

  sue = loadGIF("sprites/sue.gif");
  yoshi = loadGIF("sprites/yoshi.gif");

  font = load_font("fonts/10x20.bdf");

  outlineFont = create_outline_font(font);

  struct LedFont *tomThumbFont = load_font("fonts/tom-thumb.bdf");

  fonts[0] = font;
  fonts[1] = outlineFont;
  fonts[2] = tomThumbFont;

  atexit(exitHandler);
  signal(SIGINT, sigHandler);

  memset(&options, 0, sizeof(options));
  options.rows = 32;
  options.chain_length = 1;

  matrix = led_matrix_create_from_options(&options, &argc, &argv);
  if (matrix == NULL)
    return 1;

  canvas = led_matrix_create_offscreen_canvas(matrix);

  led_canvas_get_size(canvas, &width, &height);

  fprintf(stderr, "Size: %dx%d. Hardware gpio mapping: %s\n", width, height,
          options.hardware_mapping);

  sevenSegmentFont = load_font("fonts/7segment.bdf");
  struct LedClock *clock = createLedClock(sevenSegmentFont);

  int state = IDLE;

  struct Scroller *newYearScroller;
  newYearScroller =
      createScroller(0, width, fonts, sizeof(fonts) / sizeof(fonts[0]), NULL, 0,
                     "   "SINE(4)FONT(1)COLOR(808080)"HAPPY NEW "
                     "YEAR "COLOR(800000)GRADIENT(1)"2022"GRADIENT(0)COLOR(808080)"!!!  ");
  newYearScroller->y = 26;

  struct SetPoint fireSetPoints[] = {
      {.percent = 0, .color = {0, 0, 0}},
      {.percent = .1, .color = {0xff, 0, 0}},
      {.percent = .2, .color = {0xff, 0x80, 0}},
      {.percent = .3, .color = {0xff, 0xff, 0}},
      {.percent = 1, .color = {0xff, 0xff, 0xff}}};

  struct Gradient gradient = {0};
  gradient.nsetPoints = sizeof(fireSetPoints) / sizeof(struct SetPoint);
  gradient.setPoints = fireSetPoints;

  struct SetPoint rainbowSetPoints[] = {
      {.percent = 0, .color = {0xff, 0, 0}},
      {.percent = .16, .color = {0xff, 0x80, 0}},
      {.percent = .32, .color = {0xff, 0xff, 0}},
      {.percent = .48, .color = {0, 0xff, 0}},
      {.percent = .64, .color = {0, 0, 0xff}},
      {.percent = .80, .color = {0xff, 0, 0xff}},
      {.percent = .96, .color = {0x80, 0, 0xff}},
      {.percent = 1, .color = {0x80, 0, 0xff}}};

  struct Gradient rainbowGradient = {0};
  rainbowGradient.nsetPoints =
      sizeof(rainbowSetPoints) / sizeof(struct SetPoint);
  rainbowGradient.setPoints = rainbowSetPoints;

  struct Gradient *gradients[] = {&gradient, &rainbowGradient};
  newYearScroller->gradients = gradients;

  struct AnimationTimer newYearDisplayTimer;

  int animIndex = 0;
  struct Anim *currentAnim = NULL;

  while (1) {
    led_canvas_clear(canvas);

    if (state == IDLE) {
      if (!currentAnim && !(rand() & 0x3ff)) {
        currentAnim = &spriteAnims[animIndex];
        currentAnim->init();

        animIndex =
            (animIndex + 1) % (sizeof(spriteAnims) / sizeof(struct Anim));
      }

      advanceLedClock(clock);
      drawLedClock(canvas, clock);

      if (currentAnim) {
        if (!currentAnim->advance()) {
          currentAnim = NULL;
        } else {
          currentAnim->draw(canvas);
        }
      }

      if (isTargetTime()) {
        state = COUNTDOWN;
      }
    }

    if (state == COUNTDOWN) {
      int count = advanceCountdown();
      if (!count) {
        state = NEWYEAR;

        initFireworks();

        initTimer(&newYearScroller->animationTimer, 20);

        int newYearDisplayTime =
            debug ? 6000
                  : 10 * 60 * 1000; // 6s in debug mode, 10 min in normal mode

        initTimer(&newYearDisplayTimer, newYearDisplayTime);
      } else {
        drawCountdown(canvas, count);
      }
    }

    if (state == NEWYEAR) {
      advanceFireworks();
      drawFireworks(canvas);

      drawScroller(canvas, newYearScroller);
      advanceScroller(newYearScroller);

      if (animationReady(&newYearDisplayTimer)) {
        state = IDLE;
      }
    }

    canvas = led_matrix_swap_on_vsync(matrix, canvas);
  }
}
