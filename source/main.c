#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <cglm/cglm.h>
#include <GL/gl.h>

#include <pthread.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "coordinates.h"
#include "functions.h"
#include "window.h"
#include "shader.h"
#include "log.h"

#define STARCOUNT 256

int main(int argc, char **argv) {
    log_init();
    window_init();
    
    if (!opengl_load_functions()) {
        logprint(LOG_ERROR, "Failed to load OpenGL functions");
        exit(1);
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    initialize_shaders();

    /*
    int opt = getopt(argc, argv, "f:");
    if (opt == -1) {
        logprint(LOG_ERROR, "No input file specified");
        exit(1);
    } else if (opt == 'f') {
        logprint(LOG_INFO, "Input file: %s", optarg);
    }
    */

    /* Main Thread Start */
    size_t coords = sizeof(equatorial_pose_t) * STARCOUNT;
    equatorial_pose_t *stars = malloc(coords);
    if (!stars) {
        logprint(LOG_ERROR, "Failed to allocate memory for stars");
        exit(1);
    }
    for (size_t i = 0; i < STARCOUNT; i++) {
        stars[i].ra = (float)rand() / (float)RAND_MAX * 360.0f;
        stars[i].dec = (float)rand() / (float)RAND_MAX * 180.0f - 90.0f;
    }

    cartesian_pose_t *cartesian = malloc(sizeof(cartesian_pose_t) * STARCOUNT);
    if (!cartesian) {
        logprint(LOG_ERROR, "Failed to allocate memory for cartesian stars");
        exit(1);
    }
    for (size_t i = 0; i < STARCOUNT; i++) {
        equatorial_to_cartesian(&stars[i], &cartesian[i]);
    }

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cartesian_pose_t) * STARCOUNT, cartesian, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(cartesian_pose_t), (void *)0);
    glEnableVertexAttribArray(0);

    mat4 camera;
    glm_perspective(90.0, 1.0, 0.01, 10.0, &camera);
    glUseProgram(star_shader->shader_id);
    glUniformMatrix4fv(star_shader->transform_id, 1, GL_FALSE, &camera[0][0]);
    
    /* Render Loop */
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawArrays(GL_LINE_LOOP, 0, STARCOUNT);

        SDL_GL_SwapWindow(window->window);
    }
    /* Main Thread End */

    logprint(LOG_INFO, "Main thread ending.");
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    free(stars);
    free(cartesian);
    exit(0);
}
