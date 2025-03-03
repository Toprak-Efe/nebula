#include "shader.h"

shader_t *star_shader = NULL;
shader_t *vertex_shader = NULL;
shader_t *fragment_shader = NULL;

void initialize_shaders() {
    logprint(LOG_INFO, "Initializing shaders.");
    star_shader = malloc(sizeof(shader_t));
    vertex_shader = malloc(sizeof(shader_t));
    fragment_shader = malloc(sizeof(shader_t));
    if (!star_shader || !vertex_shader ||!fragment_shader) {
        logprint(LOG_ERROR, "Failed to allocate memory for shaders.");
        exit(1);
    }
    star_shader->transform_id = -1;
    vertex_shader->transform_id = -1;
    fragment_shader->transform_id = -1;

    const char *vertex_shader_start = (char *)_binary__vertex_start;
    size_t vertex_shader_size = (size_t) (_binary__vertex_end - _binary__vertex_start);
    const char *fragment_shader_start = (char *)_binary__fragment_start;
    size_t fragment_shader_size = (size_t) (_binary__fragment_end - _binary__fragment_start);

    char *vertex_source_str = malloc(sizeof(char) * vertex_shader_size + 1);
    memcpy(vertex_source_str, vertex_shader_start, vertex_shader_size);
    vertex_source_str[vertex_shader_size] = '\0';

    char *fragment_source_str = malloc(sizeof(char) * fragment_shader_size + 1);
    memcpy(fragment_source_str, fragment_shader_start, fragment_shader_size);
    fragment_source_str[fragment_shader_size] = '\0';

    GLuint vs = glCreateShader((GLenum) GL_VERTEX_SHADER);
    glShaderSource(vs, 1, (const GLchar * const *)& vertex_source_str, NULL);
    glCompileShader(vs);
    vertex_shader->shader_id = vs;

    GLuint fs = glCreateShader((GLenum) GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, (const GLchar * const *) &fragment_source_str, NULL);
    glCompileShader(fs);
    fragment_shader->shader_id = fs;
    
    GLint success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(vs, sizeof(info_log), NULL, info_log);
        logprint(LOG_ERROR, "Vertex shader failed to compile: %s", info_log);
    }
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(fs, sizeof(info_log), NULL, info_log);
        logprint(LOG_ERROR, "Fragment shader failed to compile: %s", info_log);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(fs, sizeof(info_log), NULL, info_log);
        logprint(LOG_ERROR, "Program failed to link: %s", info_log);
    }
    star_shader->shader_id = program;
    star_shader->transform_id = glGetUniformLocation(program, "transformation");

    free(vertex_source_str);
    free(fragment_source_str);
    atexit(uninitialize_shaders);
}

void uninitialize_shaders() {
    logprint(LOG_INFO, "Uninitializing shaders.");
    if (vertex_shader) {
        if (glIsProgram(vertex_shader->shader_id)) {
            glDeleteProgram(vertex_shader->shader_id);
        }
        free(vertex_shader);
    }
    if (fragment_shader) {
        if (glIsProgram(fragment_shader->shader_id)) {
            glDeleteProgram(fragment_shader->shader_id);
        }
        free(fragment_shader);
    }
    if (star_shader) {
        if (glIsProgram(star_shader->shader_id)) {
            glDeleteProgram(star_shader->shader_id);
        }
        free(star_shader);
    }
}
