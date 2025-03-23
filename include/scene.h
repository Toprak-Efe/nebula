#ifndef SCENE_H
#define SCENE_H

#include "log.h"
#include "window.h"
#include "camera.h"
#include "shader.h"
#include "coordinates.h"

#define STARCOUNT 32

typedef struct {
    camera_t camera;
    GLuint vao, vbo;
    vec3 pivot_pose;
    GLuint pvao, pvbo;
} scene_t;

extern scene_t *g_main_scene;

void initialize_scene(scene_t *scene);
void uninitialize_scene(scene_t *scene);
void process_input(scene_t *scene, SDL_Event *event);
void highlight_pivot(vec3 pose);
void draw_scene(scene_t *scene);
void adjust_scene(scene_t *scene);
void initialize_main_scene();
void uninitialize_main_scene();

#endif // SCENE_H
