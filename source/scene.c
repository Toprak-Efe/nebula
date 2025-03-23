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
    static bool pivot = false;
    vec4 pivot_pose;
    int x, y;
    int w, h;
    vec3 vector = {0.0f, 0.0f, 0.0f};
    switch (event->type) {
    case SDL_MOUSEMOTION:
        if ((SDL_GetMouseState(&x, &y) & SDL_BUTTON_LMASK) && pivot) {
            SDL_GetWindowSize(g_window->window, &w, &h);
            float x_norm = (((float) x / (float) w)*2.0 - 1.0);
            float y_norm = (((float) y / (float) h)*2.0 - 1.0);
            camera_inverse(&g_main_scene->camera, pivot_pose, x_norm, y_norm);

            cartesian_pose_t c_mouse, c_pivot;
            equatorial_pose_t e_mouse, e_pivot;

            c_mouse.x = pivot_pose[0];
            c_mouse.y = pivot_pose[1];
            c_mouse.z = pivot_pose[2];
            c_pivot.x = scene->pivot_pose[0];
            c_pivot.y = scene->pivot_pose[1];
            c_pivot.z = scene->pivot_pose[2];
            cartesian_to_equatorial(&c_mouse, &e_mouse);
            cartesian_to_equatorial(&c_pivot, &e_pivot);
            
            versor rotation;
            glm_vec3_zero(rotation);
            vector[2] += e_pivot.dec - e_mouse.dec;
            vector[1] += e_pivot.ra - e_mouse.ra;

            glm_euler_yxz_quat(vector, rotation);
            glm_quat_mul(rotation, scene->camera.orientation, scene->camera.orientation);
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event->button.button == SDL_BUTTON_LEFT) {
            SDL_GetMouseState(&x, &y);
            SDL_GetWindowSize(g_window->window, &w, &h);
            float x_norm = ((float) x / (float) w)*2.0 - 1.0;
            float y_norm = ((float) y / (float) h)*2.0 - 1.0;
            camera_inverse(&g_main_scene->camera, pivot_pose, x_norm, y_norm);
            glm_vec4_copy3(pivot_pose, scene->pivot_pose);
            pivot = true;
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (event->button.button == SDL_BUTTON_LEFT) {
            pivot = false;
        }
        break;
    case SDL_MOUSEWHEEL:
        scene->camera.arclength -= (event->wheel.preciseY);
        if (scene->camera.arclength < 5.0f) {
            scene->camera.arclength = 5.0f;
        }
        if (scene->camera.arclength > 60.0f) {
            scene->camera.arclength = 60.0f;
        }
        camera_project(&scene->camera);
        break;
    case SDL_KEYDOWN:
        switch (event->key.keysym.sym) {
#ifdef CAMERA_MOVEMENT_CONTROLS
        case SDLK_w:
            vector[2] = -0.01f;
            camera_move(&scene->camera, vector);
            break;
        case SDLK_s:
            vector[2] = 0.01f;
            camera_move(&scene->camera, vector);
            break;
        case SDLK_a:
            vector[0] = -0.01f;
            camera_move(&scene->camera, vector);
            break;  
        case SDLK_d:
            vector[0] = 0.01f;
            camera_move(&scene->camera, vector);
            break;
        case SDLK_ESCAPE:
            glm_vec3_zero(scene->camera.position);
            glm_quat_identity(scene->camera.orientation);
            break;
#endif
#ifndef CAMERA_MOVEMENT_CONTROLS
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(g_star_shader->shader_id); 
    
    mat4 view_transform, final_transform;
    camera_view_transform(&scene->camera, view_transform);
    glm_mat4_mul(scene->camera.projection, view_transform, final_transform);

    glUniformMatrix4fv(g_star_shader->u_trans_id, 1, GL_FALSE, &final_transform[0][0]);
    glUniform3f(g_star_shader->u_color_id, 1.0, 1.0, 1.0);
    
    glBindVertexArray(scene->vao);
    glPointSize(2.0);
    glDrawArrays(GL_POINTS, 0, STARCOUNT);

    mat4 model_transform;
    glm_mat4_identity(model_transform);
    glm_translate(model_transform, scene->pivot_pose);
    glm_mat4_mul(final_transform, model_transform, final_transform);

    glUniformMatrix4fv(g_star_shader->u_trans_id, 1, GL_FALSE, &final_transform[0][0]);
    glUniform3f(g_star_shader->u_color_id, 0.0, 1.0, 0.0);

    glBindVertexArray(scene->pvao);
    glPointSize(8.0);
    glDrawArrays(GL_POINTS, 0, 1);

    SDL_GL_SwapWindow(g_window->window);
}

void adjust_scene(scene_t *scene) {
    int w, h;
    SDL_GetWindowSize(g_window->window, &w, &h);
    glm_perspective(glm_rad(scene->camera.arclength), (float) w / (float) h, 0.01f, 1000.0f, scene->camera.projection);
}

void initialize_main_scene() {
    g_main_scene = calloc(1, sizeof(scene_t));
    if (!g_main_scene) {
        logprint(LOG_ERROR, "Failed to allocate memory for the main scene");
        exit(1);
    }
    
    glPointSize(2.0);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
        stars[i].dec =  ((-90.0f + 180.0f*i/STARCOUNT));
    }
    cartesian_pose_t *cartesian = malloc(sizeof(cartesian_pose_t) * STARCOUNT);
    if (!cartesian) {
        logprint(LOG_ERROR, "Failed to allocate memory for cartesian stars");
        exit(1);
    }
    for (size_t i = 0; i < STARCOUNT; i++) {
        equatorial_to_cartesian(&stars[i], &cartesian[i]);
    }

    GLuint vao[2], vbo[2];
    glGenBuffers(2, vbo);
    glGenVertexArrays(2, vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cartesian_pose_t) * STARCOUNT, cartesian, GL_STATIC_DRAW);
    glBindVertexArray(vao[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(cartesian_pose_t), (void *)0);
    g_main_scene->vao = vao[0];
    g_main_scene->vbo = vbo[0];

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    GLfloat pivot_star[3] = {0.0f, 0.0f, 0.0f};
    glBufferData(GL_ARRAY_BUFFER, sizeof(cartesian_pose_t), pivot_star, GL_STATIC_DRAW);
    glBindVertexArray(vao[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(cartesian_pose_t), (void *)0);
    g_main_scene->pvao = vao[1];
    g_main_scene->pvbo = vbo[1];

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
