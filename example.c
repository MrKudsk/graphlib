#include "olive.c"
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./thirdparty/stb_image_write.h"

#define WIDTH 800
#define HEIGHT 600

#define COLS 8
#define ROWS 6
#define CELL_WIDTH (WIDTH / COLS)
#define CELL_HEIGHT (HEIGHT / ROWS)

#define BACKGROUND_COLOR 0xFF202020
#define FOREGROUND_COLOR 0xFF2020FF
#define IMGS_PATH "./imgs"

static uint32_t pixels[HEIGHT * WIDTH];

#define OLIVEC_SWAP(T, a, b)                                                   \
  do {                                                                         \
    T t = a;                                                                   \
    a = b;                                                                     \
    b = t;                                                                     \
  } while (0)

void olivec_fill_circle(uint32_t *pixels, size_t pixels_width,
                        size_t pixels_height, int cx, int cy, int r,
                        uint32_t color) {
  if (r == 0)
    return;

  int x1 = cx - r;
  int x2 = cx + r;
  if (x1 > x2)
    OLIVEC_SWAP(int, x1, x2);

  int y1 = cy - r;
  int y2 = cy + r;
  if (y1 > y2)
    OLIVEC_SWAP(int, y1, y2);

  for (int y = y1; y <= y2; ++y) {
    if (0 <= y && y < (int)pixels_height) {
      for (int x = x1; x <= x2; ++x) {
        if (0 <= x && x < (int)pixels_width) {
          int dx = x - cx;
          int dy = y - cy;
          if (dx * dx + dy * dy <= r * r) {
            pixels[y * pixels_width + x] = color;
          }
        }
      }
    }
  }
}

bool checker_example(void) {
  olivec_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

  for (int y = 0; y < ROWS; ++y) {
    for (int x = 0; x < COLS; ++x) {
      uint32_t color = BACKGROUND_COLOR;
      if ((x + y) % 2 == 0) {
        color = 0xFF2020FF;
      }
      olivec_fill_rect(pixels, WIDTH, HEIGHT, x * CELL_WIDTH, y * CELL_HEIGHT,
                       CELL_WIDTH, CELL_HEIGHT, color);
    }
  }

  const char *file_path = IMGS_PATH "/checker.png";
  printf("Generated %s\n", file_path);
  if (!stbi_write_png(file_path, WIDTH, HEIGHT, 4, pixels,
                      WIDTH * sizeof(uint32_t))) {
    fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path,
            strerror(errno));
    return false;
  }
  return true;
}

float lerpf(float a, float b, float t) { return a + (b - a) * t; }

bool circle_example(void) {
  olivec_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

  for (int y = 0; y < ROWS; ++y) {
    for (int x = 0; x < COLS; ++x) {
      float u = (float)x / COLS;
      float v = (float)y / ROWS;
      float t = (u + v) / 2;

      size_t radius = CELL_WIDTH;
      if (CELL_HEIGHT < radius)
        radius = CELL_HEIGHT;

      olivec_fill_circle(pixels, WIDTH, HEIGHT, x * CELL_WIDTH + CELL_WIDTH / 2,
                         y * CELL_HEIGHT + CELL_HEIGHT / 2,
                         (size_t)lerpf(radius / 8, radius / 2, t),
                         FOREGROUND_COLOR);
    }
  }

  const char *file_path = IMGS_PATH "/circle.png";
  printf("Generated %s\n", file_path);
  if (!stbi_write_png(file_path, WIDTH, HEIGHT, 4, pixels,
                      WIDTH * sizeof(uint32_t))) {
    fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path,
            strerror(errno));
    return false;
  }
  return true;
}

int main(void) {
  if (!checker_example())
    return -1;
  if (!circle_example())
    return -1;

  return 0;
}
