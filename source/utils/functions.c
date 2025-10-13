#include "../../include/nebula/utils/functions.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define OPENGL_DEFINE(func_name, func_type)  \
  func_type func_name = NULL;

#define OPENGL_FUNCTION OPENGL_DEFINE
OPENGL_FUNCTIONS
#undef OPENGL_FUNCTION

union bridge {
    void* object_ptr;
    void (*function_ptr)(void);
};

#define OPENGL_LOAD(func_name, func_type)                \
    func_name = (func_type)(union bridge){               \
        .object_ptr = SDL_GL_GetProcAddress(#func_name)  \
    }.function_ptr;

#define OPENGL_VALIDATE(func_name, func_type)                    \
  if (func_name == NULL) {                                       \
    return false;                                                \
  }

bool opengl_load_functions(void)
{
  #define OPENGL_FUNCTION OPENGL_LOAD
  OPENGL_FUNCTIONS
  #undef OPENGL_FUNCTION

  #define OPENGL_FUNCTION OPENGL_VALIDATE
  OPENGL_FUNCTIONS
  #undef OPENGL_FUNCTION

  return true;
}
