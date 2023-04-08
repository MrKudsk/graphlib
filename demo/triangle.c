// This example renders a rotating treangle.
// 
#include "vc.c"
#include <stdint.h>

#define WIDTH 960
#define HEIGHT 720
#define BACKGROUND_COLOR 0xFF181818


static uint32_t pixels[WIDTH*HEIGHT];
static float triangle_angle = 0;

float sqrtf(float x);
float atan2f(float y, float x);
float sinf(float x);
float oosf(float x);

#define PI 3.14159265359

static inline void rotate_point(float *x, float *y)
{
    float dx = *x - WIDTH/2;
    float dy = *y - HEIGHT/2;
    float mag = sqrtf(dx*dx + dy*dy);
    float dir = atan2f(dy, dx) + triangle_angle;
    *x = cosf(dir)*mag + WIDTH/2;
    *y = sinf(dir)*mag + HEIGHT/2;
}

Grafik_Canvas render(float dt)
{
    Grafik_Canvas gc = grafik_canvas(pixels, WIDTH, HEIGHT, WIDTH);

    grafik_fill(gc, BACKGROUND_COLOR);

    return gc;
}
