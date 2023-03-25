#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "graphlib.c"

#define WIDTH 800
#define HEIGHT 600

void graphlib_fill_rect(uint32_t *pixels, size_t width, size_t height, int x0, int y0, size_t w, size_t h, uint32_t color)
{
    for (int dy = 0; dy < (int) h; ++dy) {
        int y = y0 + dy;
        if (0 <= y && y < (int) height) {
            for (int dx = 0; dx < (int) w; ++dx) {
                int x = x0 + dx;
                if (0 <= x && x < (int) width) {
                    pixels[y*width + x] = color;
                }
            }
        }
    }
}

static uint32_t pixels[HEIGHT*WIDTH];

int main(void)
    {
    graphlib_fill(pixels, WIDTH, HEIGHT, 0xFF202020);
    int rw = 50*4;
    int rh = 30*4;
    graphlib_fill_rect(pixels, WIDTH, HEIGHT, WIDTH/2 - rw/2, HEIGHT/2 - rh/2, rw, rh, 0xFF2020FF);
    const char *file_path = "output.ppm";
    Errno err = graphlib_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return 1;
    }
    printf("Hello, World\n");
    return 0;
}
