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
#include "log.h"

#define STARCOUNT 1024

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

    /* Main Thread Start */
    size_t coords = sizeof(equatorial_pose_t) * STARCOUNT;
    equatorial_pose_t *stars = malloc(coords);
    if (!stars) {
        logprint(LOG_ERROR, "Failed to allocate memory for stars");
        exit(1);
    }
    for (size_t i = 0; i < STARCOUNT; i++) {
        stars[i].r = 1.0f;
        stars[i].ra = (float)rand() / (float)RAND_MAX * 10.0f - 5.0f;
        stars[i].dec = (float)rand() / (float)RAND_MAX * 360.0f;
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

    camera_t camera;
    camera_init(&camera);
    
    /* Render Loop */
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            int lr, ud;
            vec3 delta = {0.0f, 0.0f, 0.0f};
            switch (event.type) {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_MOUSEMOTION:
                lr = event.motion.xrel;
                ud = -event.motion.yrel;
                logprint(LOG_INFO, "Mouse Input: %i, %i", lr, ud);
                camera.direction.dec -= ud / 5.0f;
                camera.direction.ra -= lr / 5.0f;
                if (camera.direction.dec > 45.0f) {
                    camera.direction.dec = 45.0f;
                }
                if (camera.direction.dec < -45.0f) {
                    camera.direction.dec = -45.0f;
                }
                if (camera.direction.ra > 360.0f) {
                    camera.direction.ra -= 360.0f;
                }
                if (camera.direction.ra < -360.0f) {
                    camera.direction.ra += 360.0f;
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_w:
                    delta[0] = 0.01f;
                    camera_move(&camera, delta);
                    break;
                case SDLK_s:
                    delta[0] = -0.01f;
                    camera_move(&camera, delta);
                    break;
                case SDLK_a:
                    delta[1] = 0.01f;
                    camera_move(&camera, delta);
                    break;
                case SDLK_d:
                    delta[1] = -0.01f;
                    camera_move(&camera, delta);
                    break;
                case SDLK_ESCAPE:
                    camera.position.x = 0.0f;
                    camera.position.y = 0.0f;
                    camera.position.z = 0.0f;
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }

        mat4 view_transform, final_transform;
        camera_view_transform(&camera, view_transform);
        glm_mul(axis_remapping_matrix, view_transform, final_transform);
        glm_mul(camera.projection, final_transform, final_transform);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);
        glUseProgram(star_shader->shader_id);
        glUniformMatrix4fv(star_shader->transform_id, 1, GL_FALSE, &final_transform[0][0]);
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
