#include "thirdparty/arena.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h> 
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define return_defere(value) do { result = (value); goto defer; } while (0)
#define UNUSED(x) (void)(x)
#define UNIMPLEMENTED(message) \
    do { \
        fprintf(stderr, "%s:%d: UNIMPLEMENTED: %s\n", __FILE__, __LINE__, message); \
        exit(1); \
    } while (0)
#define UNREACHABLE(message) \
    do { \
        fprintf(stderr, "%s:%d: UNREACHABLE: %s\n", __FILE__, __LINE__, message); \
        exit(1); \
    } while (0)

#define ARENA_IMPLEMENTATION
#include "./arena.h"

static Arena default_arena = {0};
static Arena *context_arena = &default_arena;

static void *context_alloc(size_t size)
{
    assert(context_arena);
    return arena_alloc(context_arena, size);
}

static void *context_realloc(void *oldp, size_t oldsz, size_t newsz)
{
    if (newsz <= oldsz) return oldp;
    return memcpy(context_alloc(newsz), oldp, oldsz);
}

#define STBI_MALLOC context_alloc
#define STBI_FREE UNUSED
#define STBI_REALLOC_SIZED context_realloc
#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"

#define STBIW_MALLOC STBI_MALLOC
#define STBIW_FREE STBI_FREE
#define STBIW_REALLOC_SIZED STBI_REALLOC_SIZED
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb_image_write.h"

#define GRAFIK_IMPLEMENTATION
#include "grafik.c"

#define BACKGROUND_COLOR 0xFF202020
#define FOREGROUND_COLOR 0xFF2020FF
#define RED_COLOR 0xFF2020AA
#define GREEN_COLOR 0xFF20AA20
#define BLUE_COLOR 0xFFAA2020
#define ERROR_COLOR 0xFFFF00FF

#define TEST_DIR_PATH "./test"

bool canvas_load(const char *file_path, Grafik_Canvas *gc)
{
    int width, height;
    uint32_t *pixels = (uint32_t*) stbi_load(file_path, &width, &height, NULL, 4);
    if (pixels == NULL) return false;
    *gc = grafik_canvas(pixels, width, height, width);
    return true;
}

bool canvas_save(Grafik_Canvas gc, const char *file_path)
{
    return stbi_write_png(file_path, gc.width, gc.height, 4, gc.pixels, sizeof(uint32_t)*gc.stride);
}

bool record_test_case(Grafik_Canvas actual_canvas, const char *expected_file_path)
{
    if (!canvas_save(actual_canvas, expected_file_path)) {
        fprintf(stderr, "ERROR: could not write file %s: %s\n", expected_file_path, strerror(errno));
        return(false);
    }
    printf("Generated %s\n", expected_file_path);
    return(true);
}

Grafik_Canvas canvas_alloc(size_t width, size_t height)
{
    uint32_t *pixels = context_alloc(sizeof(uint32_t)*width*height);
    return grafik_canvas(pixels, width, height, width);
}

typedef enum {
    REPLAY_PASSED,
    REPLAY_FAILED,
    REPLAY_ERRORED,
} Replay_Result;

static inline size_t min_size(size_t a, size_t b)
{
    if (a < b) return a;
    return b;
}

static inline size_t max_size(size_t a, size_t b)
{
    if (a > b) return a;
    return b;
}

Replay_Result replay_test_case(const char *program_path, Grafik_Canvas actual_canvas, const char *expected_file_path, const char *actual_file_path, const char *diff_file_path)
{
    Grafik_Canvas expected_canvas;
    if (!canvas_load(expected_file_path, &expected_canvas)) {
        fprintf(stderr, "%s: ERROR: could not read the file: %s\n", expected_file_path, strerror(errno));
        if (errno == ENOENT) {
            fprintf(stderr, "%s: HINT: Consider running `$ %s record` to create it\n", expected_file_path, program_path);
        }
        return(REPLAY_ERRORED);
    }

    bool failed = false;

    if (expected_canvas.width != actual_canvas.width || expected_canvas.height != actual_canvas.height) {
        failed = true;
    }

    Grafik_Canvas diff_canvas =
        canvas_alloc(
            max_size(expected_canvas.width, actual_canvas.width),
            max_size(expected_canvas.height, actual_canvas.height));
    grafik_fill(diff_canvas, ERROR_COLOR);

    for (size_t y = 0; y < min_size(expected_canvas.height, actual_canvas.height); ++y) {
        for (size_t x = 0; x < min_size(expected_canvas.width, actual_canvas.width); ++x) {
            uint32_t expected_pixel = GRAFIK_PIXEL(expected_canvas, x, y);
            uint32_t actual_pixel = GRAFIK_PIXEL(actual_canvas, x, y);
            if (expected_pixel != actual_pixel) {
                GRAFIK_PIXEL(diff_canvas, x, y) = ERROR_COLOR;
                failed = true;
            } else {
                GRAFIK_PIXEL(diff_canvas, x, y) = expected_pixel;
            }
        }
    }

    if (failed) {

        if (!canvas_save(actual_canvas, actual_file_path)) {
            fprintf(stderr, "ERROR: could not write image file with actual pixels %s: %s\n", actual_file_path, strerror(errno));
            return(REPLAY_ERRORED);
        }

        if (!canvas_save(diff_canvas, diff_file_path)) {
            fprintf(stderr, "ERROR: could not wrilte diff image file %s: %s\n", diff_file_path, strerror(errno));
            return(REPLAY_ERRORED);
        }

        fprintf(stderr, "%s: TEST FAILURE: unexpected pixels in generated image\n", expected_file_path);
        fprintf(stderr, "%s:   Expected: %s\n", expected_file_path, expected_file_path);
        fprintf(stderr, "%s:   Actual:   %s\n", expected_file_path, actual_file_path);
        fprintf(stderr, "%s:   Diff:     %s\n", expected_file_path, diff_file_path);
        fprintf(stderr, "%s: HINT: If this behaviour is intentional confirm that by updating the image with `$ %s record`\n", expected_file_path, program_path);
        return(REPLAY_FAILED);
    }

    printf("%s: OK\n", expected_file_path);

    return(REPLAY_PASSED);
}

typedef struct {
    Grafik_Canvas (*generate_actual_canvas)(void);
    const char *expected_file_path;
    const char *actual_file_path;
    const char *diff_file_path;
} Test_Case;

#define DEFINE_TEST_CASE(name) \
    { \
        .generate_actual_canvas = name, \
        .expected_file_path = TEST_DIR_PATH "/" #name "_expected.png", \
        .actual_file_path = TEST_DIR_PATH "/" #name "_actual.png", \
        .diff_file_path = TEST_DIR_PATH "/" #name "_diff.png", \
    }

Grafik_Canvas test_fill_rect(void)
{
    size_t width = 128;
    size_t height = 128;
    Grafik_Canvas gc = canvas_alloc(width, height);
    grafik_fill(gc, BACKGROUND_COLOR);
    grafik_rect(gc, width/2 - width/8, height/2 - height/8, width/4, height/4, RED_COLOR);
    grafik_rect(gc, width - 1, height - 1, -width/2, -height/2, GREEN_COLOR);
    grafik_rect(gc, -width/4, -height/4, width/2, height/2, BLUE_COLOR);
    return gc;
}

Grafik_Canvas test_fill_circle(void)
{
    size_t width = 128;
    size_t height = 128;
    Grafik_Canvas oc = canvas_alloc(width, height);
    grafik_fill(oc, BACKGROUND_COLOR);
    grafik_circle(oc, 0, 0, width/2, RED_COLOR);
    grafik_circle(oc, width/2, height/2, width/4, BLUE_COLOR);
    grafik_circle(oc, width*3/4, height*3/4, -width/4, GREEN_COLOR);
    return oc;
}

Grafik_Canvas test_draw_line(void)
{
    size_t width = 128;
    size_t height = 128;
    Grafik_Canvas gc = canvas_alloc(width, height);
    grafik_fill(gc, BACKGROUND_COLOR);
    grafik_line(gc, 0, 0, width, height, RED_COLOR);
    grafik_line(gc, width, 0, 0, height, BLUE_COLOR);
    grafik_line(gc, width/2, 0, width/2, height, GREEN_COLOR);
    return gc;
}

Grafik_Canvas test_fill_triangle(void)
{
    size_t width = 128;
    size_t height = 128;
    Grafik_Canvas gc = canvas_alloc(width, height);
    grafik_fill(gc, BACKGROUND_COLOR);
    
    {
        int x1 = width/2,   y1 = height/8;
        int x2 = width/8,   y2 = height/2;
        int x3 = width*7/8, y3 = height*7/8;
        grafik_triangle(gc, x1, y1, x2, y2, x3, y3, RED_COLOR);
        }

    {
        int x1 = width/2,   y1 = height*2/8;
        int x2 = width*2/8,   y2 = height/2;
        int x3 = width*6/8, y3 = height/2;
        grafik_triangle(gc, x1, y1, x2, y2, x3, y3, GREEN_COLOR);
    }

    {
        int x1 = width/8,   y1 = height/8;
        int x2 = width/8,   y2 = height*3/8;
        int x3 = width*3/8, y3 = height*3/8;
        grafik_triangle(gc, x1, y1, x2, y2, x3, y3, BLUE_COLOR);
    }

    return gc;
}

Grafik_Canvas test_alpha_blending(void)
{
    size_t width = 128;
    size_t height = 128;
    Grafik_Canvas gc = canvas_alloc(width, height);
    grafik_fill(gc, BACKGROUND_COLOR);
    grafik_rect(gc, 0, 0, width*3/4, height*3/4, RED_COLOR);
    grafik_rect(gc, width-1, height-1, -width*3/4, -height*3/4, 0x5520AA20);
    grafik_circle(gc, width/2, height/2, width/4, 0xBBAA2020);
    grafik_triangle(gc, 0, height, width, height, width/2, 0, 0xBB20AAAA);
    return gc;
}

Grafik_Canvas test_checker_example(void)
{
    size_t width = 800/2;
    size_t height = 600/2;
    int cols = (8*2);
    int rows = (6*2);
    int cell_width = (width/cols);
    int cell_height = (height/rows);
    Grafik_Canvas gc = canvas_alloc(width, height);

    grafik_fill(gc, BACKGROUND_COLOR);
    
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            uint32_t color = BACKGROUND_COLOR;
            if ((x + y)%2 == 0) {
                color = 0xFF2020FF;
            }
            grafik_rect(gc, x*cell_height, y*cell_height, cell_width, cell_height, color);
        }
    }
    return gc;
}

Grafik_Canvas test_circle_example(void)
{
    size_t width = 800/2;
    size_t height = 600/2;
    int cols = (8*2);
    int rows = (6*2);
    int cell_width = (width/cols);
    int cell_height = (height/rows);
    Grafik_Canvas gc = canvas_alloc(width, height);

    grafik_fill(gc, BACKGROUND_COLOR);
    
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            float u = (float)x/cols;
            float v = (float)y/rows;
            float t = (u + v)/2;

            int radius = cell_width;
            if (cell_height < radius) radius = cell_height;

                grafik_circle(gc, x*cell_width + cell_width/2, y*cell_height + cell_height/2,
                          (size_t) (radius/8*(1-t) + radius/2*t),
                          FOREGROUND_COLOR);
        } 
    }
    return gc;
}

Grafik_Canvas test_lines_circle(void)
{
    size_t width = 800/2;
    size_t height = 600/2;
    Grafik_Canvas gc = canvas_alloc(width, height);
    grafik_fill(gc, BACKGROUND_COLOR);
    
    size_t n = 20;
    float angle = 2*M_PI/n;
    float length = width;
    if (length > height) length = height;
    length /= 3;
    float x1 = width/2;
    float y1 = height/2;
    for (size_t i = 0; i < n; ++i) {
        float x2 = x1 + cosf(angle*i)*length;
        float y2 = y1 + sinf(angle*i)*length;
        grafik_line(gc, x1, y1, x2, y2, 0xFF1818FF);
    }
    return gc;
}

Grafik_Canvas test_lines_example(void)
{
    size_t width = 800/2;
    size_t height = 600/2;
    Grafik_Canvas gc = canvas_alloc(width, height);
    grafik_fill(gc, BACKGROUND_COLOR);
    
    grafik_line(gc, 0, 0, width, height, FOREGROUND_COLOR);
    grafik_line(gc, width, 0, 0, height, FOREGROUND_COLOR);
    grafik_line(gc, 0, 0, width/4, height, 0xFF20FF20);
    grafik_line(gc, width/4, 0, 0, height, 0xFF20FF20);
    grafik_line(gc, width, 0, width/4*3, height, 0xFF20FF20);
    grafik_line(gc, width/4*3, 0, width, height, 0xFF20FF20);
    grafik_line(gc, 0, height/2, width, height/2, 0xFFFF3030);
    grafik_line(gc, width/2, 0, width/2, height, 0xFFFF3030);

    return gc;
}

Grafik_Canvas test_hello_world_text_rendering(void)
{
    size_t size = 5;
        const char *text = "hello world";
    size_t text_len = strlen(text);

        size_t width = 800;
    size_t height = 300/2;
    Grafik_Canvas gc = canvas_alloc(width, height);
    grafik_fill(gc, BACKGROUND_COLOR);
    
    grafik_text(gc, text, width/2 - DEFAULT_FONT_WIDTH*size*text_len/2,
                height/2 - DEFAULT_FONT_HEIGHT*size/2, default_font, size, 
                FOREGROUND_COLOR);

    return gc;
}


Test_Case test_cases[] = {
    DEFINE_TEST_CASE(test_fill_rect),
    DEFINE_TEST_CASE(test_fill_circle),
    DEFINE_TEST_CASE(test_draw_line),
    DEFINE_TEST_CASE(test_fill_triangle),
    DEFINE_TEST_CASE(test_alpha_blending),
    DEFINE_TEST_CASE(test_checker_example),
    DEFINE_TEST_CASE(test_circle_example),
    DEFINE_TEST_CASE(test_lines_circle),
    DEFINE_TEST_CASE(test_lines_example),
    DEFINE_TEST_CASE(test_hello_world_text_rendering),
    };
#define TEST_CASES_COUNT (sizeof(test_cases)/sizeof(test_cases[0]))

int main(int argc, char **argv)
{
    int result = 0;

    assert(argc >= 1);
    const char *program_path = argv[0];
    bool record = argc >= 2 && strcmp(argv[1], "record") == 0;

    for (size_t i = 0; i < TEST_CASES_COUNT; ++i) {
        Grafik_Canvas actual_canvas = test_cases[i].generate_actual_canvas();
        if (record) {
            if (!record_test_case(actual_canvas, test_cases[i].expected_file_path)) return_defer(1);
        } else {
            if (replay_test_case(program_path, actual_canvas, test_cases[i].expected_file_path, test_cases[i].actual_file_path, test_cases[i].diff_file_path) == REPLAY_ERRORED) return_defer(1);
        }
        arena_reset(&default_arena);
    }
defer:
    arena_free(&default_arena);
    return result;
}
