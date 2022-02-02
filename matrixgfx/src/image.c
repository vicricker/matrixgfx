#include <matrixgfx.h>
#include <stdlib.h>

void freeImage(struct Image *image) {
  if (image) {
    if (image->bitmaps) {
      free(image->bitmaps);
    }
    free(image);
  }
}

Pixel *getFrame(struct Image *image, unsigned int frame) {
  frame = frame % image->imageCount; // Wrap if out of bounds

  return image->bitmaps + (image->width * image->height * frame);
}
