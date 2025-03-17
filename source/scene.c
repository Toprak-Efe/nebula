#include "scene.h"

scene_t *g_main_scene = NULL;

void initialize_scene(scene_t *scene) {
    initialize_camera(&scene->camera);
}

void uninitialize_scene(scene_t *scene) {
    if (scene) {
        free(scene);
    }
}

void process_input(scene_t *scene, SDL_Event *event) {
    vec3 pivot_p;
    uint8_t pivot = false;
    int x, y;
    int w, h;
    vec3 vector = {0.0f, 0.0f, 0.0f};
    switch (event->type) {
    case SDL_MOUSEMOTION:
        if (SDL_GetMouseState(&x, &y) & SDL_BUTTON_LMASK & pivot) {
            /* Pivot the pivot point. */
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event->button.button == SDL_BUTTON_LEFT) {
            SDL_GetMouseState(&x, &y);
            SDL_GetWindowSize(g_window->window, &w, &h);
            float x_norm = ((float) x / (float) w)*2.0 - 1.0;
            float y_norm = ((float) y / (float) h)*2.0 - 1.0;
            camera_inverse(&g_main_scene->camera, pivot_p, x_norm, y_norm);
            pivot = true;
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (event->button.button == SDL_BUTTON_LEFT) {
            /* Motion animation */
        }
        break;
    case SDL_KEYDOWN:
        switch (event->key.keysym.sym) {
#ifdef DEBUG
        case SDLK_w:
            vector[2] = -0.01f;
            camera_move(&g_main_scene->camera, vector);
            break;
        case SDLK_s:
            vector[2] = 0.01f;
            camera_move(&g_main_scene->camera, vector);
            break;
        case SDLK_a:
            vector[0] = -0.01f;
            camera_move(&g_main_scene->camera, vector);
            break;  
        case SDLK_d:
            vector[0] = 0.01f;
            camera_move(&g_main_scene->camera, vector);
            break;
        case SDLK_ESCAPE:
            g_main_scene->camera.position.x = 0.0f;
            g_main_scene->camera.position.y = 0.0f;
            g_main_scene->camera.position.z = 0.0f;
            break;
#endif
#ifndef DEBUG
        case SDLK_ESCAPE:
            initialize_camera(&g_main_scene->camera);
            break;
#endif
        default:
            break;
        }
    default:
        break;
    }
}

void highlight_pivot(vec3 pose) {
    
}

void draw_scene(scene_t *scene) {
    mat4 view_transform, final_transform;
    camera_view_transform(&scene->camera, view_transform);
    glm_mat4_mul(scene->camera.projection, view_transform, final_transform);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(scene->vao);
    glUseProgram(g_star_shader->shader_id);
    glUniformMatrix4fv(g_star_shader->transform_id, 1, GL_FALSE, &final_transform[0][0]);
    glDrawArrays(GL_POINTS, 0, STARCOUNT);
    SDL_GL_SwapWindow(g_window->window);
}

void adjust_scene(scene_t *scene) {
    int w, h;
    SDL_GetWindowSize(g_window->window, &w, &h);
    glm_perspective(glm_rad(scene->camera.arclength), (float) w / (float) h, 0.01f, 1000.0f, scene->camera.projection);
}

void initialize_main_scene() {
    g_main_scene = malloc(sizeof(scene_t));
    if (!g_main_scene) {
        logprint(LOG_ERROR, "Failed to allocate memory for the main scene");
        exit(1);
    }
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    size_t coords = sizeof(equatorial_pose_t) * STARCOUNT;
    equatorial_pose_t *stars = malloc(coords);
    if (!stars) {
        logprint(LOG_ERROR, "Failed to allocate memory for stars");
        exit(1);
    }
    for (size_t i = 0; i < STARCOUNT; i++) {
        stars[i].r = 1.0f;
        stars[i].ra = (-90.0f + 180.0f*i/STARCOUNT);
        stars[i].dec =  ((-90.0f + 180.0f*i/STARCOUNT)*45.05);
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
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cartesian_pose_t) * STARCOUNT, cartesian, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(cartesian_pose_t), (void *)0);
    g_main_scene->vao = vao;
    g_main_scene->vbo = vbo;

    initialize_camera(&g_main_scene->camera);
    free(stars);
    free(cartesian);
    atexit(uninitialize_main_scene);
}

void uninitialize_main_scene() {
    if (g_main_scene){
        free(g_main_scene);
        glDeleteVertexArrays(1, &g_main_scene->vao);
        glDeleteBuffers(1, &g_main_scene->vbo);
    }
}
