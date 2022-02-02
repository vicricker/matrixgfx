#include <time.h>
#include <stdlib.h>
#include <matrixgfx.h>

void initTimer(struct AnimationTimer *timer, clock_t delayMs) {
  timer->delayTicks = delayMs * CLOCKS_PER_SEC / 1000;
  timer->startTime = clock();
}

int animationReady(struct AnimationTimer *timer) {
  int ready = 0;

  clock_t now = clock();

  clock_t elapsed = now - timer->startTime;

  if (abs(elapsed) >= timer->delayTicks) {
    timer->startTime = now - (elapsed - timer->delayTicks);
    ready = 1;
  }

  return ready;
}
