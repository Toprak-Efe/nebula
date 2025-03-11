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
    int lr, ud;
    vec3 delta = {0.0f, 0.0f, 0.0f};
    switch (event->type) {
    case SDL_MOUSEMOTION:
        lr = event->motion.xrel;
        ud = -event->motion.yrel;
        g_main_scene->camera.direction.dec -= ud / 5.0f;
        g_main_scene->camera.direction.ra -= lr / 5.0f;
        if (g_main_scene->camera.direction.dec > 45.0f) {
            g_main_scene->camera.direction.dec = 45.0f;
        }
        if (g_main_scene->camera.direction.dec < -45.0f) {
            g_main_scene->camera.direction.dec = -45.0f;
        }
        if (g_main_scene->camera.direction.ra > 360.0f) {
            g_main_scene->camera.direction.ra -= 360.0f;
        }
        if (g_main_scene->camera.direction.ra < -360.0f) {
            g_main_scene->camera.direction.ra += 360.0f;
        }
        break;
    case SDL_KEYDOWN:
        switch (event->key.keysym.sym) {
        case SDLK_w:
            delta[0] = 0.01f;
            camera_move(&g_main_scene->camera, delta);
            break;
        case SDLK_s:
            delta[0] = -0.01f;
            camera_move(&g_main_scene->camera, delta);
            break;
        case SDLK_a:
            delta[1] = 0.01f;
            camera_move(&g_main_scene->camera, delta);
            break;
        case SDLK_d:
            delta[1] = -0.01f;
            camera_move(&g_main_scene->camera, delta);
            break;
        case SDLK_ESCAPE:
            g_main_scene->camera.position.x = 0.0f;
            g_main_scene->camera.position.y = 0.0f;
            g_main_scene->camera.position.z = 0.0f;
            break;
        default:
            break;
        }
    default:
        break;
    }
}

void draw_scene(scene_t *scene) {
    mat4 view_transform, final_transform;
    camera_view_transform(&scene->camera, view_transform);
    glm_mul(axis_remapping_matrix, view_transform, final_transform);
    glm_mul(scene->camera.projection, final_transform, final_transform);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(scene->vao);
    glUseProgram(g_star_shader->shader_id);
    glUniformMatrix4fv(g_star_shader->transform_id, 1, GL_FALSE, &final_transform[0][0]);
    glDrawArrays(GL_LINE_LOOP, 0, STARCOUNT);
    SDL_GL_SwapWindow(g_window->window);
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
        stars[i].ra = ((float)rand() / (float)RAND_MAX * 360.0f);
        stars[i].dec = ((float)rand() / (float)RAND_MAX * 360.0f);
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
