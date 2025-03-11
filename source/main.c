#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <cglm/cglm.h>
#include <GL/gl.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "coordinates.h"
#include "functions.h"
#include "camera.h"
#include "window.h"
#include "shader.h"
#include "scene.h"
#include "log.h"

int main(int argc, char **argv) {
    log_init();
    
    window_init();
    if (!opengl_load_functions()) {
        logprint(LOG_ERROR, "Failed to load OpenGL functions.");
        exit(1);
    }
    initialize_shaders();
    initialize_main_scene();
    
    /* Application Loop */
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_MOUSEMOTION:
                process_input(g_main_scene, &event);
            case SDL_KEYDOWN:
                process_input(g_main_scene, &event);
            default:
                break;
            }
        }
        draw_scene(g_main_scene);
    }

    logprint(LOG_INFO, "Exiting application.");
    exit(0);
}
