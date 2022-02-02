/**
 * @file plasma.c
 * @brief Base on the code from https://www.pygame.org/pcr/numpy_plasma/index.php by
 * Gareth Noyce (g at korruptor.demon.co.uk)
 * @version 0.1
 * @date 2022-01-28
 */

#include "plasma.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define COS_TAB_SIZE 256

struct Plasma *createPlasma(int width, int height) {
  struct Plasma *plasma = calloc(1, sizeof(struct Plasma));

  plasma->width = width;
  plasma->height = height;

  plasma->cos_tab = malloc(COS_TAB_SIZE);

  make_cosine(plasma->cos_tab, COS_TAB_SIZE);

  return plasma;
}

void freePlasma(struct Plasma *plasma) {
  free(plasma->cos_tab);
  free(plasma);
}

void make_cosine(uint8_t *arr, size_t size) {
  for (size_t i = 0; i < size; i++) {
    *arr++ = 78 * (cos((double)i * M_PI / 128));
  }
}

void add_cosine(struct Plasma *plasma, uint8_t *buffer) {
  uint8_t t1 = plasma->i0;
  uint8_t t2 = plasma->i1;

  for (int y = 0; y < plasma->height; y++) {
    uint8_t t3 = plasma->i2;
    uint8_t t4 = plasma->i3;

    for (int x = 0; x < plasma->width; x++) {
      *buffer++ = plasma->cos_tab[t1] + plasma->cos_tab[t2] +
                  plasma->cos_tab[t3] + plasma->cos_tab[t4];

      t3 += 3;
      t2 += 2;
    }

    t1 += 1;
    t2 += 1;
  }

  plasma->i0 += 1;
  plasma->i1 += 2;
  plasma->i2 -= 3;
  plasma->i3 += 4;
}
