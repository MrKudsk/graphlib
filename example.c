#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "graphlib.c"

#define WIDTH 800
#define HEIGHT 600

static uint32_t pixels[HEIGHT*WIDTH];


int main(void)
    {
    graphlib_fill(pixels, WIDTH, HEIGHT, 0xFF0000FF)    ;
    const char *file_path = "output.ppm";
    Errno err = graphlib_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return 1;
    }
    printf("Hello, World\n");
    return 0;
}
