
#define GRAFIK_IMPLEMENTATION
#include <grafik.c>

Grafik_Canvas render(float dt);

#define WASM_PLATFORM 0
#define SDL_PLATFORM 1
#define TERM_PLATFORM 2

#ifndef SDL_PLATFORM
#error "Please define PLATFORM macro"
#endif

#if PLATFORM == SDL_PLATFORM
#include <stdio.h>
#include <SDL2/SDL.h>

SDL_Texture *texture = NULL;
size_t actual_width = 0;
size_t actual_height = 0;

#define return_defer(value) do { result = (value); goto defer; } while (0)

bool resize_texture(SDL_Renderer *renderer, size_t new_width, size_t new_height)
{
    if (texture != NULL) SDL_DestroyTexture(texture);
    actual_width = new_width;
    actual_height = new_height;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, actual_width, actual_height);
    if (texture == NULL) return false;
    return true;
}


#endif // SDL_PlatfoRm
//
