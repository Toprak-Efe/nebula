#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

namespace nebula::windows {

struct SDLWindow {
    SDL_Window *window = NULL;
    SDL_GLContext context = NULL;
    void initialize();
    void draw();
    ~SDLWindow();
};

};

