#ifndef SHADER_H
#define SHADER_H

#include <stdlib.h>
#include <GL/gl.h>

#include "functions.h"
#include "log.h"

typedef struct {
    GLuint shader_id;
    GLint u_trans_id;
    GLint u_color_id;
} shader_t;

extern const unsigned char _binary__vertex_start[];
extern const unsigned char _binary__vertex_end[];
extern const unsigned char _binary__vertex_size[];
extern const unsigned char _binary__geometry_start[];
extern const unsigned char _binary__geometry_end[];
extern const unsigned char _binary__geometry_size[];
extern const unsigned char _binary__fragment_start[];
extern const unsigned char _binary__fragment_end[];
extern const unsigned char _binary__fragment_size[];

extern shader_t *g_star_shader;
extern shader_t *g_vertex_shader;
extern shader_t *g_geometry_shader;
extern shader_t *g_fragment_shader;

void initialize_shaders();
void uninitialize_shaders();

#endif // SHADER_H
