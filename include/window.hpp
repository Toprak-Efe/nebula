#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <string>
#include <memory>
#include <log.hpp>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>


extern "C" {
    #include <functions.h> 
};

namespace astronomy::windows {

struct SDLWindow {
    SDL_Window *window = NULL;
    SDL_GLContext context = NULL;
    void initialize();
    ~SDLWindow();
};

inline SDLWindow mainWindow;

};

#endif // WINDOW_HPP
