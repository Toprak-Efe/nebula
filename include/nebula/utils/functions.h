#pragma once

#include <GL/gl.h>
#include <stdbool.h>

#define OPENGL_FUNCTIONS                                                          \
  OPENGL_FUNCTION(glCreateShader, PFNGLCREATESHADERPROC)                          \
  OPENGL_FUNCTION(glDeleteShader, PFNGLDELETESHADERPROC)                          \
  OPENGL_FUNCTION(glAttachShader, PFNGLATTACHSHADERPROC)                          \
  OPENGL_FUNCTION(glDetachShader, PFNGLDETACHSHADERPROC)                          \
  OPENGL_FUNCTION(glShaderSource, PFNGLSHADERSOURCEPROC)                          \
  OPENGL_FUNCTION(glCompileShader, PFNGLCOMPILESHADERPROC)                        \
  OPENGL_FUNCTION(glGetShaderiv, PFNGLGETSHADERIVPROC)                            \
  OPENGL_FUNCTION(glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC)                  \
  OPENGL_FUNCTION(glCreateProgram, PFNGLCREATEPROGRAMPROC)                        \
  OPENGL_FUNCTION(glDeleteProgram, PFNGLDELETEPROGRAMPROC)                        \
  OPENGL_FUNCTION(glUseProgram, PFNGLUSEPROGRAMPROC)                              \
  OPENGL_FUNCTION(glLinkProgram, PFNGLLINKPROGRAMPROC)                            \
  OPENGL_FUNCTION(glValidateProgram, PFNGLVALIDATEPROGRAMPROC)                    \
  OPENGL_FUNCTION(glGetProgramiv, PFNGLGETPROGRAMIVPROC)                          \
  OPENGL_FUNCTION(glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC)                \
  OPENGL_FUNCTION(glUniform1i, PFNGLUNIFORM1IPROC)                                \
  OPENGL_FUNCTION(glUniform1f, PFNGLUNIFORM1FPROC)                                \
  OPENGL_FUNCTION(glUniform3f, PFNGLUNIFORM3FPROC)                                \
  OPENGL_FUNCTION(glUniformMatrix4fv, PFNGLUNIFORMMATRIX4FVPROC)                  \
  OPENGL_FUNCTION(glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC)              \
  OPENGL_FUNCTION(glGenBuffers, PFNGLGENBUFFERSPROC)                              \
  OPENGL_FUNCTION(glDeleteBuffers, PFNGLDELETEBUFFERSPROC)                        \
  OPENGL_FUNCTION(glBindBuffer, PFNGLBINDBUFFERPROC)                              \
  OPENGL_FUNCTION(glBufferData, PFNGLBUFFERDATAPROC)                              \
  OPENGL_FUNCTION(glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC)                    \
  OPENGL_FUNCTION(glDeleteVertexArrays, PFNGLDELETEVERTEXARRAYSPROC)              \
  OPENGL_FUNCTION(glBindVertexArray, PFNGLBINDVERTEXARRAYPROC)                    \
  OPENGL_FUNCTION(glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC)            \
  OPENGL_FUNCTION(glEnableVertexAttribArray, PFNGLENABLEVERTEXATTRIBARRAYPROC)    \
  OPENGL_FUNCTION(glDisableVertexAttribArray, PFNGLDISABLEVERTEXATTRIBARRAYPROC)  \
  OPENGL_FUNCTION(glIsProgram, PFNGLISPROGRAMPROC)                                \
  OPENGL_FUNCTION(glIsShader, PFNGLISSHADERPROC)                                  \
  OPENGL_FUNCTION(glGenerateMipmap, PFNGLGENERATEMIPMAPPROC)                                  

#define OPENGL_DECLARE(func_name, func_type)  \
    extern func_type func_name;

#define OPENGL_FUNCTION OPENGL_DECLARE
OPENGL_FUNCTIONS
#undef OPENGL_FUNCTION

 bool opengl_load_functions(void);

