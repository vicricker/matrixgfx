#include <led-matrix-c.h>
#include <stdlib.h>
#include "ledclock.h"

struct LedClock *createLedClock(struct LedFont *font) {
  struct LedClock *ledClock = calloc(1, sizeof(struct LedClock));

  ledClock->font = font;
  initTimer(&ledClock->animationTimer, 1000);

  return ledClock;
}

void advanceLedClock(struct LedClock *clock) {
  if (animationReady(&clock->animationTimer)) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    snprintf(clock->text, sizeof(clock->text), "%02d:%02d:%02d",
             timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
  }
}

void drawLedClock(struct LedCanvas *canvas, struct LedClock *clock) {
  draw_text(canvas, clock->font, 0, 29, 0x20, 0, 0, "88:88:88", 0);

  draw_text(canvas, clock->font, 0, 29, 0xff, 0, 0, clock->text, 0);
}

void freeLedClock(struct LedClock *clock) {
  free(clock);
}
