#include <gif_lib.h>
#include <stdio.h>
#include <matrixgfx.h>
#include <assert.h>
#include <stdlib.h>

Pixel convertPixel(GifColorType gifColorType, bool transparent) {
  Pixel result;

  if (transparent) {
    result.val = 0;
  } else {
    result.rgba.a = 0xff;
    result.rgba.r = gifColorType.Red;
    result.rgba.g = gifColorType.Green;
    result.rgba.b = gifColorType.Blue;
  }

  return result;
}

struct Image *loadGIF(char *fileName) {
  int err;
  GifFileType *f = DGifOpenFileName(fileName, &err);

  assert(f != NULL);
  int ret = DGifSlurp(f);
  assert(ret == GIF_OK);
  int width = f->SWidth;
  int height = f->SHeight;

#ifdef DEBUG
  printf("Background: %d\n", f->SBackGroundColor);
  printf("Total Frames:     %d\n", f->ImageCount);
  printf("Image Size:       %dx%d\n", width, height);
  printf("Color Resolution: %d\n", f->SColorResolution);
#endif

  GifColorType *colorMap = f->SColorMap->Colors;

  struct Image *i = malloc(sizeof(struct Image));

  i->width = f->SWidth;
  i->height = f->SHeight;
  i->imageCount = f->ImageCount;

  i->bitmaps = malloc(i->width * i->height * i->imageCount * sizeof(Pixel));

  assert(i->bitmaps != NULL);

  Pixel *pixels = i->bitmaps;

  for (int frame = 0; frame < f->ImageCount; frame++) {
    GraphicsControlBlock gcb;

    DGifSavedExtensionToGCB(f, frame, &gcb);

    GifByteType *bits = f->SavedImages[frame].RasterBits;
    for (int x = 0; x < width * height; x++) {

      GifByteType index = *bits++;
      const GifColorType color = colorMap[index];

      *pixels++ = convertPixel(color, index == gcb.TransparentColor);
    }
  }

  DGifCloseFile(f, &err);

  return i;
}
