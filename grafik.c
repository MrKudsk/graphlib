// Grafik  
// a basic graphical library writen in C
//

#ifndef GRAFIK_C_
#define GRAFIK_C_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef GRAFIKDEF
#define GRAFIKDEF static inline
#endif

#ifndef GRAFIK_AA_RES
#define GRAFIK_AA_RES 2
#endif

#define return_defer(value) do { result = (value); goto defer; } while (0)
#define GRAFIK_SWAP(T, a, b) do { T t = a; a = b; b = t; } while (0)
#define GRAFIK_SIGN(T, x) ((T)((x) > 0) - (T)((x) < 0))
#define GRAFIK_ABS(T, x) (GRAFIK_SIGN(T, x)*(x))

typedef struct {
    size_t width, height;
    const char *glyphs;
} Grafik_Font;

#define DEFAULT_FONT_HEIGHT 8
#define DEFAULT_FONT_WIDTH 6
static char default_font_glyphs[128][DEFAULT_FONT_HEIGHT][DEFAULT_FONT_WIDTH] = {
    ['a'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 1},
        {0, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {0, 1, 1, 1, 1},
        {0, 0, 0, 0, 0},
        },
    ['b'] = {
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        },
    ['c'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 1},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        },
    ['d'] = {
        {0, 0, 0, 0, 1},
        {0, 0, 0, 0, 1},
        {0, 1, 1, 0, 1},
        {1, 0, 0, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {0, 1, 1, 1, 1},
        {0, 0, 0, 0, 0},
        },
    ['e'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0},
        {0, 1, 1, 1, 1},
        {0, 0, 0, 0, 0},
        },
    ['f'] = {
        {0, 0, 0, 1, 1},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 1},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
            },
    ['g'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {0, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 0},
        },
    ['h'] = {
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0},
        },
    ['i'] = {
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0},
        },
    ['j'] = {
        {0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1},
        {0, 0, 0, 0, 1},
        {0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {0, 1, 1, 1, 0},
        },
    ['k'] = {
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 1, 0, 0},
        {1, 1, 0, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {0, 0, 0, 0, 0},
        },
    ['l'] = {
        {0, 1, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0},
            },
    ['m'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {1, 1, 0, 1, 0},
        {1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0},
        },
    ['n'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {1, 1, 1, 1, 0},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0},
        },
    ['o'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        },
    ['p'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 0},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0},
        },
    ['q'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 1},
        {1, 0, 0, 1, 1},
        {1, 0, 0, 0, 1},
        {0, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {0, 0, 0, 0, 1},
        },
    ['r'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 0, 1},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        },
    ['s'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1},
        {1, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        },
    ['t'] = {
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0},
        },
    ['u'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        },
    ['v'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        },
    ['w'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1},
        {0, 1, 1, 1, 1},
        {0, 0, 0, 0, 0},
        },
    ['x'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        },
    ['y'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        },
    ['z'] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        },
};

static Grafik_Font default_font = {
    .glyphs = &default_font_glyphs[0][0][0],
    .width = DEFAULT_FONT_WIDTH,
    .height = DEFAULT_FONT_HEIGHT,
};

typedef struct {
    uint32_t *pixels;
    size_t width;
    size_t height;
    size_t stride;
} Grafik_Canvas;

#define GRAFIK_CANVAS_NULL ((Grafik_Canvas) {0})
#define GRAFIK_PIXEL(gc, x, y) (gc).pixels[(y)*(gc).stride + (x)]

GRAFIKDEF Grafik_Canvas grafik_canvas(uint32_t *pixels, size_t width, size_t height, size_t stride);
GRAFIKDEF Grafik_Canvas grafik_subcanvas(Grafik_Canvas gc, int x, int y, int w, int h);
GRAFIKDEF void grafik_blend_color(uint32_t *c1, uint32_t c2);
GRAFIKDEF void grafik_fill(Grafik_Canvas gc, uint32_t color);
GRAFIKDEF void grafik_rect(Grafik_Canvas gc, int x, int y, int w, int h, uint32_t color);
GRAFIKDEF void grafik_circle(Grafik_Canvas oc, int cx, int cy, int r, uint32_t color);
// TODO: lines with different thiccness
GRAFIKDEF void grafik_line(Grafik_Canvas oc, int x1, int y1, int x2, int y2, uint32_t color);
GRAFIKDEF void grafik_triangle(Grafik_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
GRAFIKDEF void grafik_triangle3(Grafik_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t c1, uint32_t c2, uint32_t c3);
GRAFIKDEF void grafik_text(Grafik_Canvas oc, const char *text, int x, int y, Grafik_Font font, size_t size, uint32_t color);

// The point of this function is to produce two ranges x1..x2 and y1..y2 that are guaranteed to be safe to iterate over the canvas of size pixels_width by pixels_height without any boundary checks.
//
// if (grafik_normalize_rect(x, y, w, h, WIDTH, HEIGHT, &x1, &y1, &x2, &y2)) {
//     for (int x = x1; x <= x2; ++x) {
//         for (int y = y1; y <= y2; ++y) {
//             GRAFIK_PIXEL(oc, x, y) = 0x69696969;
//         }
//     }
// } else {
    //     // Rectangle is invisible cause it's completely out-of-bounds
// }
GRAFIKDEF bool grafik_normalize_rect(int x, int y, int w, int h, size_t pixels_width, size_t pixels_height, int *x1, int *x2, int *y1, int *y2);

#endif // GRAfik_C_

#ifdef GRAFIK_IMPLEMENTATION

GRAFIKDEF Grafik_Canvas grafik_canvas(uint32_t *pixels, size_t width, size_t height, size_t stride)
{
    Grafik_Canvas gc = {
        .pixels = pixels,
        .width = width,
        .height = height,
        .stride = stride,
    };
    return gc;
}

GRAFIKDEF bool grafik_normalize_rect(int x, int y, int w, int h,
                                     size_t pixels_width, size_t pixels_height,
                                     int *x1, int *x2, int *y1, int *y2)
{
    *x1 = x;
    *y1 = y;

    // Convert the rectangle to 2-points representation
    *x2 = *x1 + GRAFIK_SIGN(int, w)*(GRAFIK_ABS(int, w) - 1);
    if (*x1 > *x2) GRAFIK_SWAP(int, *x1, *x2);
    *y2 = *y1 + GRAFIK_SIGN(int, h)*(GRAFIK_ABS(int, h) - 1);
    if (*y1 > *y2) GRAFIK_SWAP(int, *y1, *y2);

    // Cull out invisible rectangle
    if (*x1 >= (int) pixels_width) return false;
    if (*x2 < 0) return false;
    if (*y1 >= (int) pixels_height) return false;
    if (*y2 < 0) return false;

    // Clamp the rectangle to the boundaries
    if (*x1 < 0) *x1 = 0;
    if (*x2 >= (int) pixels_width) *x2 = (int) pixels_width - 1;
    if (*y1 < 0) *y1 = 0;
    if (*y2 >= (int) pixels_height) *y2 = (int) pixels_height - 1;

    return true;
}

GRAFIKDEF Grafik_Canvas grafik_subcanvas(Grafik_Canvas gc, int x, int y, int w, int h)
{
    int x1, x2, y1, y2;
    if (!grafik_normalize_rect(x, y, w, h, gc.width, gc.height, &x1, &x2, &y1, &y2)) return GRAFIK_CANVAS_NULL;
    gc.pixels = &GRAFIK_PIXEL(gc, x1, y1);
    gc.width = x2 - x1 + 1;
    gc.height = y2 - y1 + 1;
    return gc;
}

// TODO: custom pixel formats
// Maybe we can store pixel format info in Grafik_Canvas
#define GRAFIK_RED(color)   (((color)&0x000000FF)>>(8*0))
#define GRAFIK_GREEN(color) (((color)&0x0000FF00)>>(8*1))
#define GRAFIK_BLUE(color)  (((color)&0x00FF0000)>>(8*2))
#define GRAFIK_ALPHA(color) (((color)&0xFF000000)>>(8*3))
#define GRAFIK_RGBA(r, g, b, a) ((((r)&0xFF)<<(8*0)) | (((g)&0xFF)<<(8*1)) | (((b)&0xFF)<<(8*2)) | (((a)&0xFF)<<(8*3)))

GRAFIKDEF void grafik_blend_color(uint32_t *c1, uint32_t c2)
{
    uint32_t r1 = GRAFIK_RED(*c1);
    uint32_t g1 = GRAFIK_GREEN(*c1);
    uint32_t b1 = GRAFIK_BLUE(*c1);
    uint32_t a1 = GRAFIK_ALPHA(*c1);

    uint32_t r2 = GRAFIK_RED(c2);
    uint32_t g2 = GRAFIK_GREEN(c2);
    uint32_t b2 = GRAFIK_BLUE(c2);
    uint32_t a2 = GRAFIK_ALPHA(c2);

    r1 = (r1*(255 - a2) + r2*a2)/255; if (r1 > 255) r1 = 255;
    g1 = (g1*(255 - a2) + g2*a2)/255; if (g1 > 255) g1 = 255;
    b1 = (b1*(255 - a2) + b2*a2)/255; if (b1 > 255) b1 = 255;

    *c1 = GRAFIK_RGBA(r1, g1, b1, a1);
}

GRAFIKDEF void grafik_fill(Grafik_Canvas gc, uint32_t color)
{
    for (size_t y = 0; y < gc.height; ++y) {
        for (size_t x = 0; x < gc.width; ++x) {
            GRAFIK_PIXEL(gc, x, y) = color;
            }
    }
}

// grafik_fill_rect
// Is filling a rectael at x and y positions, in defined color.
GRAFIKDEF void grafik_rect(Grafik_Canvas gc, int x, int y, int w, int h, uint32_t color)
{
    int x1, x2, y1, y2;
    if (!grafik_normalize_rect(x, y, w, h, gc.width, gc.height, &x1, &x2, &y1, &y2)) return;
    for (int x = x1; x <= x2; ++x) {
        for (int y = y1; y <= y2; ++y) {
            grafik_blend_color(&GRAFIK_PIXEL(gc, x, y), color);
        }
    }
}


GRAFIKDEF void grafik_circle(Grafik_Canvas oc, int cx, int cy, int r, uint32_t color)
{
    int x1, y1, x2, y2;
    int r1 = r + GRAFIK_SIGN(int, r);
    if (!grafik_normalize_rect(cx - r1, cy - r1, 2*r1, 2*r1, oc.width, oc.height, &x1, &x2, &y1, &y2)) return;

    for (int y = y1; y <= y2; ++y) {
        for (int x = x1; x <= x2; ++x) {
            int count = 0;
            for (int sox = 0; sox < GRAFIK_AA_RES; ++sox) {
                for (int soy = 0; soy < GRAFIK_AA_RES; ++soy) {
                    // TODO: switch to 64 bits to make the overflow less likely
                    // Also research the probability of overflow
                    int res1 = (GRAFIK_AA_RES + 1);
                    int dx = (x*res1*2 + 2 + sox*2 - res1*cx*2 - res1);
                    int dy = (y*res1*2 + 2 + soy*2 - res1*cy*2 - res1);
                    if (dx*dx + dy*dy <= res1*res1*r*r*2*2) count += 1;
                }
            }
            uint32_t alpha = ((color&0xFF000000)>>(3*8))*count/GRAFIK_AA_RES/GRAFIK_AA_RES;
            uint32_t updated_color = (color&0x00FFFFFF)|(alpha<<(3*8));
            grafik_blend_color(&GRAFIK_PIXEL(oc, x, y), updated_color);
        }
    }
}

// TODO: AA for line
GRAFIKDEF void grafik_line(Grafik_Canvas oc, int x1, int y1, int x2, int y2, uint32_t color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    if (dx == 0 && dy == 0) {
        grafik_blend_color(&GRAFIK_PIXEL(oc, x1, y1), color);
        return;
    }

    if (GRAFIK_ABS(int, dx) > GRAFIK_ABS(int, dy)) {
        if (x1 > x2) {
            GRAFIK_SWAP(int, x1, x2);
            GRAFIK_SWAP(int, y1, y2);
        }
        for (int x = x1; x <= x2; ++x) {
            int y = dy*(x - x1)/dx + y1;
            // TODO: move boundary checks out side of the loops in grafik_draw_line
            if (0 <= x && x < (int) oc.width && 0 <= y && y < (int) oc.height) {
                grafik_blend_color(&GRAFIK_PIXEL(oc, x, y), color);
            }
        }
    } else {
        if (y1 > y2) {
            GRAFIK_SWAP(int, x1, x2);
            GRAFIK_SWAP(int, y1, y2);
        }
        for (int y = y1; y <= y2; ++y) {
            int x = dx*(y - y1)/dy + x1;
            if (0 <= x && x < (int) oc.width && 0 <= y && y < (int) oc.height) {
                grafik_blend_color(&GRAFIK_PIXEL(oc, x, y), color);
            }
        }
    }
}

uint32_t mix_colors3(uint32_t c1, uint32_t c2, uint32_t c3, int t1, int t2, int t3, int den)
{
    // TODO: estimate how much overflows are an issue in integer only environment
    int64_t r1 = GRAFIK_RED(c1);
    int64_t g1 = GRAFIK_GREEN(c1);
    int64_t b1 = GRAFIK_BLUE(c1);
    int64_t a1 = GRAFIK_ALPHA(c1);

    int64_t r2 = GRAFIK_RED(c2);
    int64_t g2 = GRAFIK_GREEN(c2);
    int64_t b2 = GRAFIK_BLUE(c2);
    int64_t a2 = GRAFIK_ALPHA(c2);

    int64_t r3 = GRAFIK_RED(c3);
    int64_t g3 = GRAFIK_GREEN(c3);
    int64_t b3 = GRAFIK_BLUE(c3);
    int64_t a3 = GRAFIK_ALPHA(c3);

    if (den != 0) {
        int64_t r4 = (r1*t1 + r2*t2 + r3*t3)/den;
        int64_t g4 = (g1*t1 + g2*t2 + g3*t3)/den;
        int64_t b4 = (b1*t1 + b2*t2 + b3*t3)/den;
        int64_t a4 = (a1*t1 + a2*t2 + a3*t3)/den;

        return GRAFIK_RGBA(r4, g4, b4, a4);
    }

    return 0;
}

void barycentric(int x1, int y1, int x2, int y2, int x3, int y3,
                 int xp, int yp,
                 int *u1, int *u2, int *det)
{
    *det = ((x1 - x3)*(y2 - y3) - (x2 - x3)*(y1 - y3));
    *u1  = ((y2 - y3)*(xp - x3) + (x3 - x2)*(yp - y3));
    *u2  = ((y3 - y1)*(xp - x3) + (x1 - x3)*(yp - y3));
    // u3 = det - u1 - u2
}

GRAFIKDEF void grafik_triangle3(Grafik_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3,
                                uint32_t c1, uint32_t c2, uint32_t c3)
{
    if (y1 > y2) {
        GRAFIK_SWAP(int, x1, x2);
        GRAFIK_SWAP(int, y1, y2);
        GRAFIK_SWAP(int, c1, c2);
    }

    if (y2 > y3) {
        GRAFIK_SWAP(int, x2, x3);
        GRAFIK_SWAP(int, y2, y3);
        GRAFIK_SWAP(int, c2, c3);
    }

    if (y1 > y2) {
        GRAFIK_SWAP(int, x1, x2);
        GRAFIK_SWAP(int, y1, y2);
        GRAFIK_SWAP(int, c1, c2);
    }

    int dx12 = x2 - x1;
    int dy12 = y2 - y1;
    int dx13 = x3 - x1;
    int dy13 = y3 - y1;

    for (int y = y1; y <= y2; ++y) {
        // TODO: move boundary checks outside of loops in grafik_fill_triangle
        if (0 <= y && (size_t) y < oc.height) {
            int s1 = dy12 != 0 ? (y - y1)*dx12/dy12 + x1 : x1;
            int s2 = dy13 != 0 ? (y - y1)*dx13/dy13 + x1 : x1;
            if (s1 > s2) GRAFIK_SWAP(int, s1, s2);
            for (int x = s1; x <= s2; ++x) {
                if (0 <= x && (size_t) x < oc.width) {
                    int u1, u2, det;
                    barycentric(x1, y1, x2, y2, x3, y3, x, y, &u1, &u2, &det);
                    uint32_t color = mix_colors3(c1, c2, c3, u1, u2, det - u1 - u2, det);
                    grafik_blend_color(&GRAFIK_PIXEL(oc, x, y), color);
                }
            }
        }
    }

    int dx32 = x2 - x3;
    int dy32 = y2 - y3;
    int dx31 = x1 - x3;
    int dy31 = y1 - y3;

    for (int y = y2; y <= y3; ++y) {
        if (0 <= y && (size_t) y < oc.height) {
            int s1 = dy32 != 0 ? (y - y3)*dx32/dy32 + x3 : x3;
            int s2 = dy31 != 0 ? (y - y3)*dx31/dy31 + x3 : x3;
            if (s1 > s2) GRAFIK_SWAP(int, s1, s2);
            for (int x = s1; x <= s2; ++x) {
                if (0 <= x && (size_t) x < oc.width) {
                    int u1, u2, det;
                    barycentric(x1, y1, x2, y2, x3, y3, x, y, &u1, &u2, &det);
                    uint32_t color = mix_colors3(c1, c2, c3, u1, u2, det - u1 - u2, det);
                    grafik_blend_color(&GRAFIK_PIXEL(oc, x, y), color);
                }
            }
        }
    }
}

// TODO: AA for triangle
GRAFIKDEF void grafik_triangle(Grafik_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
{
    if (y1 > y2) {
        GRAFIK_SWAP(int, x1, x2);
        GRAFIK_SWAP(int, y1, y2);
    }

    if (y2 > y3) {
        GRAFIK_SWAP(int, x2, x3);
        GRAFIK_SWAP(int, y2, y3);
    }

    if (y1 > y2) {
        GRAFIK_SWAP(int, x1, x2);
        GRAFIK_SWAP(int, y1, y2);
    }

    int dx12 = x2 - x1;
    int dy12 = y2 - y1;
    int dx13 = x3 - x1;
    int dy13 = y3 - y1;

    for (int y = y1; y <= y2; ++y) {
        // TODO: move boundary checks outside of loops in grafik_fill_triangle
        if (0 <= y && (size_t) y < oc.height) {
            int s1 = dy12 != 0 ? (y - y1)*dx12/dy12 + x1 : x1;
            int s2 = dy13 != 0 ? (y - y1)*dx13/dy13 + x1 : x1;
            if (s1 > s2) GRAFIK_SWAP(int, s1, s2);
            for (int x = s1; x <= s2; ++x) {
                if (0 <= x && (size_t) x < oc.width) {
                    grafik_blend_color(&GRAFIK_PIXEL(oc, x, y), color);
                }
            }
        }
    }

    int dx32 = x2 - x3;
    int dy32 = y2 - y3;
    int dx31 = x1 - x3;
    int dy31 = y1 - y3;

    for (int y = y2; y <= y3; ++y) {
        if (0 <= y && (size_t) y < oc.height) {
            int s1 = dy32 != 0 ? (y - y3)*dx32/dy32 + x3 : x3;
            int s2 = dy31 != 0 ? (y - y3)*dx31/dy31 + x3 : x3;
            if (s1 > s2) GRAFIK_SWAP(int, s1, s2);
            for (int x = s1; x <= s2; ++x) {
                if (0 <= x && (size_t) x < oc.width) {
                    grafik_blend_color(&GRAFIK_PIXEL(oc, x, y), color);
                }
            }
        }
    }
}

GRAFIKDEF void grafik_text(Grafik_Canvas oc, const char *text, int tx, int ty, Grafik_Font font, size_t glyph_size, uint32_t color)
{
    for (size_t i = 0; *text; ++i, ++text) {
        int gx = tx + i*font.width*glyph_size;
        int gy = ty;
        const char *glyph = &font.glyphs[(*text)*sizeof(char)*font.width*font.height];
        for (int dy = 0; (size_t) dy < font.height; ++dy) {
            for (int dx = 0; (size_t) dx < font.width; ++dx) {
                int px = gx + dx*glyph_size;
                int py = gy + dy*glyph_size;
                if (0 <= px && px < (int) oc.width && 0 <= py && py < (int) oc.height) {
                    if (glyph[dy*font.width + dx]) {
                        grafik_rect(oc, px, py, glyph_size, glyph_size, color);
                    }
                }
            }
        }
    }
}

// TODO: grafik_copy does not work correctly with dst out of bounds
// TODO: bilinear interpolation for grafik_copy
void grafik_copy(Grafik_Canvas src, Grafik_Canvas dst)
{
    for (size_t y = 0; y < dst.height; ++y) {
        for (size_t x = 0; x < dst.width; ++x) {
            size_t nx = x*src.width/dst.width;
            size_t ny = y*src.height/dst.height;
            GRAFIK_PIXEL(dst, x, y) = GRAFIK_PIXEL(src, nx, ny);
        }
    }
}

#endif // GRAFIK_IMPLEMENTATION

// TODO: 3D triangles with z-buffering
// TODO: 3D textures
// TODO: Stencil
// TODO: Benchmarking
// TODO: SIMD implementations
// TODO: olivec_ring
// TODO: olivec_ellipse
// TODO: bezier curves

