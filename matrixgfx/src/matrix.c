#include <led-matrix-c.h>
#include <matrixgfx.h>
#include <stdlib.h>

void blit(struct LedCanvas *canvas, struct Image *image, unsigned int frame,
          int x, int y) {
  Pixel *p = getFrame(image, frame);

  for (int yo = 0; yo < image->height; yo++) {
    for (int xo = 0; xo < image->width; xo++) {
      Pixel pixel = *p++;

      struct RGBA rgba = pixel.rgba;

      if (rgba.a > 0) {
        led_canvas_set_pixel(canvas, x + xo, y + yo, rgba.r, rgba.g, rgba.b);
      }
    }
  }
}

void blitFlipHoriz(struct LedCanvas *canvas, struct Image *image,
                   unsigned int frame, int x, int y) {
  Pixel *p = getFrame(image, frame);

  for (int yo = 0; yo < image->height; yo++) {
    for (int xo = image->width - 1; xo >= 0; xo--) {
      Pixel pixel = *p++;

      struct RGBA rgba = pixel.rgba;

      if (rgba.a > 0) {
        led_canvas_set_pixel(canvas, x + xo, y + yo, rgba.r, rgba.g, rgba.b);
      }
    }
  }
}

void blitFlip(struct LedCanvas *canvas, struct Image *image, unsigned int frame,
              int x, int y, int flip) {
  flip ? blitFlipHoriz(canvas, image, frame, x, y)
       : blit(canvas, image, frame, x, y);
}

void blitIndexedImage(struct LedCanvas *canvas, struct IndexedImage *image,
                      struct Color *palette, int x, int y) {
  uint8_t *bitmap = image->bitmap;

  for (int yo = 0; yo < image->height; yo++) {
    for (int xo = 0; xo < image->width; xo++) {
      struct Color *color = &palette[*bitmap++];

      led_canvas_set_pixel(canvas, x + xo, y + yo, color->r, color->g,
                           color->b);
    }
  }
}

void freeIndexedImage(struct IndexedImage *image) {
  free(image->bitmap);
  free(image);
}