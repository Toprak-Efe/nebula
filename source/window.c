#include "window.h"

window_t *window = NULL;

void window_init() {
    window = malloc(sizeof(window_t));
    if (!window) {
        logprint(LOG_ERROR, "Failed to allocate memory for window");
        exit(1);
    }
    window->window = NULL;
    window->context = NULL;

    logprint(LOG_INFO, "Initializing SDL");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        logprint(LOG_ERROR, "Failed to initialize SDL: %s", SDL_GetError());
        exit(1);
    }

    logprint(LOG_INFO, "Platform: %s", SDL_GetPlatform());
    logprint(LOG_INFO, "CPU count: %d", SDL_GetCPUCount());
    logprint(LOG_INFO, "RAM: %d MB", SDL_GetSystemRAM());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    window->window = SDL_CreateWindow(
        "Astronomy",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        512, 512, 
        SDL_WINDOW_OPENGL
    );
    if (!window->window) {
        logprint(LOG_ERROR, "Failed to create window: %s", SDL_GetError());
        exit(1);
    }

    window->context = SDL_GL_CreateContext(window->window);
    if (!window->context) {
        logprint(LOG_ERROR, "Failed to create OpenGL context: %s", SDL_GetError());
        exit(1);
    }

    logprint(LOG_INFO, "OpenGL Vendor: %s", glGetString(GL_VENDOR));
    logprint(LOG_INFO, "OpenGL Version: %s", glGetString(GL_VERSION));
    logprint(LOG_INFO, "OpenGL Renderer: %s", glGetString(GL_RENDERER));
    logprint(LOG_INFO, "OpenGL Shading Language Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    if (SDL_GL_SetSwapInterval(-1) < 0) {
        SDL_GL_SetSwapInterval(1);
    }

    atexit(window_uninit);
}

void window_uninit() {
    if (window) {
        if (window->context) {
            SDL_GL_DeleteContext(window->context);
        }
        if (window->window) {
            SDL_DestroyWindow(window->window);
        }
        free(window);
    }
    SDL_Quit();
}