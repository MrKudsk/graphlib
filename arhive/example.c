#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "graphlib.c"

#define WIDTH 800
#define HEIGHT 600

#define COLS 8
#define ROWS 6
#define CELL_WIDTH (WIDTH/COLS)
#define CELL_HEIGHT (HEIGHT/ROWS)

#define BACKGROUND_COLOR 0xFF202020
#define FOREGROUND_COLOR 0xFF2020FF

static uint32_t pixels[HEIGHT*WIDTH];

bool checker_example(void)
{
    graphlib_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

    for (int y = 0; y < ROWS; ++y) {
       for (int x = 0; x < COLS; ++x) {
            uint32_t color = BACKGROUND_COLOR;
            if ((x + y)%2 == 0) {
                color = FOREGROUND_COLOR;
            }
            graphlib_fill_rect(pixels, WIDTH, HEIGHT, x*CELL_WIDTH, y*CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, color);
        }
    }
    const char *file_path = "checker.ppm";
    Errno err = graphlib_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return false;
    }
    return true;
}

bool circle_example(void)
{
    graphlib_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

    for (int y = 0; y < ROWS; ++y) {
       for (int x = 0; x < COLS; ++x) {
            size_t radius = CELL_WIDTH;
            if (CELL_HEIGHT < radius) radius = CELL_HEIGHT;
                graphlib_fill_circle(pixels, WIDTH, HEIGHT, x*CELL_WIDTH + CELL_WIDTH/2, y*CELL_HEIGHT + CELL_HEIGHT/2, radius/2, FOREGROUND_COLOR);
        }
    }

    const char *file_path = "circle.ppm";
    Errno err = graphlib_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return false;
    }
    return true;
}

bool lines_example(void) 
{
    graphlib_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

    graphlib_draw_line(pixels, WIDTH, HEIGHT,
                       0, 0, WIDTH, HEIGHT,
                           FOREGROUND_COLOR);

    graphlib_draw_line(pixels, WIDTH, HEIGHT,
                       WIDTH, 0, 0, HEIGHT,
                           FOREGROUND_COLOR);

    graphlib_draw_line(pixels, WIDTH, HEIGHT,
                       0, 0, WIDTH/4, HEIGHT,
                           0xFF20FF20);

    graphlib_draw_line(pixels, WIDTH, HEIGHT,
                       WIDTH/4, 0, 0, HEIGHT,
                           0xFF20FF20);

    graphlib_draw_line(pixels, WIDTH, HEIGHT,
                       WIDTH, 0, WIDTH/4*3, HEIGHT,
                           0xFF20FF20);

    graphlib_draw_line(pixels, WIDTH, HEIGHT,
                       0, HEIGHT/2, WIDTH, HEIGHT/2,
                           0xFFFF3030);

    graphlib_draw_line(pixels, WIDTH, HEIGHT,
                       WIDTH/2, 0, WIDTH/2, HEIGHT,
                           0xFFFF3030);

    const char *file_path = "lines.ppm";
    Errno err = graphlib_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return false;
    }
    return true;
}

int main(void)
{
    printf("Running program:\n");

    if (!checker_example()) return -1;
    if (!circle_example()) return -1;
    if (!lines_example()) return -1;
    return 0;
}
