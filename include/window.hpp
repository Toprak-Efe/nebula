#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <log.hpp>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

extern "C" {
    #include <functions.h> 
};

namespace nebula::windows {

struct SDLWindow {
    SDL_Window *window = NULL;
    SDL_GLContext context = NULL;
    void initialize();
    void draw();
    ~SDLWindow();
};

inline SDLWindow mainWindow;

};

#endif // WINDOW_HPP
