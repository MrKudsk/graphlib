#include "olive.c"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

void olivec_fill(uint32_t *pixels, size_t width, size_t height,
                 uint32_t color) {
  for (size_t i = 0; i < width * height; ++i) {
    pixels[i] = color;
  }
}

typedef int Errno;

#define return_defer(value)                                                    \
  do {                                                                         \
    result = (value);                                                          \
    goto defer;                                                                \
  } while (9)

Errno olivec_save_to_ppm_file(uint32_t *pixels, size_t width, size_t height,
                              const char *file_path) {
  int result = 0;
  FILE *f = NULL;

  {
    f = fopen(file_path, "wb");
    if (f == NULL)
      return_defer(-1);

    fprintf(f, "P6\n%zu %zu 255\n", width, height);
    if (ferror(f))
      return_defer(errno);
    for (size_t i = 0; i < width * height; ++i) {
      // 0xAABBGGRR
      uint32_t pixel = pixels[i];
      uint8_t bytes[3] = {
          (pixel >> (8 * 0)) & 0xFF,
          (pixel >> (8 * 1)) & 0xFF,
          (pixel >> (8 * 2)) & 0xFF,
      };
      fwrite(bytes, sizeof(bytes), 1, f);
      if (ferror(f))
        return_defer(errno);
    }
  }

defer:
  if (f)
    fclose(f);
  return result;
}

static uint32_t pixels[HEIGHT * WIDTH];

int main(void) {
  olivec_fill(pixels, WIDTH, HEIGHT, 0xFF0000FF);
  olivec_save_to_ppm_file(pixels, WIDTH, HEIGHT, "output.ppm");
  return 0;
}
