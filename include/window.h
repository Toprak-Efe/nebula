#ifndef WINDOW_h
#define WINDOW_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "log.h"

typedef struct {
    SDL_Window *window;
    SDL_GLContext context;
} window_t;

extern window_t *window;

void window_init();
void window_uninit();

#endif // WINDOW_h
