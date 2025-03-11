#ifndef SCENE_H
#define SCENE_H

#include "log.h"
#include "window.h"
#include "camera.h"
#include "shader.h"
#include "coordinates.h"

#define STARCOUNT 1024

typedef struct {
    camera_t camera;
    GLuint vao, vbo;
} scene_t;

extern scene_t *g_main_scene;

void initialize_scene(scene_t *scene);
void uninitialize_scene(scene_t *scene);
void process_input(scene_t *scene, SDL_Event *event);
void draw_scene(scene_t *scene);
void initialize_main_scene();
void uninitialize_main_scene();

#endif // SCENE_H
