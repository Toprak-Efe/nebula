#ifndef TEXTURES_HPP
#define TEXTURES_HPP

#include <log.hpp>
#include "SDL_opengl.h"
#include <GL/gl.h>
#include <filesystem>

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string_view>
#include <map>

namespace nebula::resources {

class TextureManager {
public:
    void initialize();
    TextureManager();
    ~TextureManager();
    GLuint getTexture(const std::string_view &name);
private:
    GLuint loadTexture(const std::string_view &path);
    std::map<std::string_view, GLuint> m_textures;
}; // class TextureManager

inline TextureManager textureManager; 

} // namespace nebula::resources

#endif//TEXTURES_HPP
