#ifndef MESHES_H
#define MESHES_H

#include <GL/gl.h>
#include "shader.h"

struct meshes_primitive_t {
    GLuint vbo, vao; 
    shader_t *shader;
};



#endif//MESHES_H
