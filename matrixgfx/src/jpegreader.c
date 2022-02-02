// Note jpeglib is stupid and must come after stdlib or this won't compile
#include <matrixgfx.h>
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

struct Image *loadJPEG(char *filename) {
  struct jpeg_decompress_struct info;
  struct jpeg_error_mgr err;

  unsigned long int width, height;

  FILE *fHandle;

  fHandle = fopen(filename, "rb");

  info.err = jpeg_std_error(&err);
  jpeg_create_decompress(&info);

  jpeg_stdio_src(&info, fHandle);
  jpeg_read_header(&info, TRUE);

  jpeg_start_decompress(&info);
  width = info.output_width;
  height = info.output_height;

  struct Image *i = malloc(sizeof(struct Image));

  i->width = width;
  i->height = height;
  i->imageCount = 1;

  printf("%ldx%ld\n", width, height);

  Pixel *bitmaps = malloc(i->width * i->height * i->imageCount * sizeof(Pixel));

  i->bitmaps = bitmaps;

  unsigned char *buffer = malloc(width * 3);

  unsigned char *rowBuffer[1] = {buffer};

  Pixel *b = bitmaps;
  while (info.output_scanline < info.output_height) {
    // printf("%d\n" , info.output_scanline);

    jpeg_read_scanlines(&info, rowBuffer, 1);

    unsigned char *src = buffer;

    for (int i = 0; i < width; i++) {
      struct RGBA *rgba = &b->rgba;

      rgba->a = 255;

      rgba->r = *src++;
      rgba->g = *src++;
      rgba->b = *src++;

      b++;
    }
  }

  free(buffer);
  jpeg_finish_decompress(&info);
  jpeg_destroy_decompress(&info);
  fclose(fHandle);

  return i;
}
