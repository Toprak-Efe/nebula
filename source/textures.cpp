#include <textures.hpp>

using namespace astronomy::resources;

TextureManager::TextureManager() {
    return;
}

TextureManager::~TextureManager() {
    return;
}

GLuint TextureManager::getTexture(const std::string_view &name) {
    if (!m_textures.contains(name)) {
        return m_textures["missingTexture"];
    }
    return m_textures[name];
}

void TextureManager::initialize() {
    static constexpr auto textureList = std::to_array({
        std::string_view("star.png"),
    }); 

    // Implement missing texture.
    GLubyte missingTexture[] = {0xBD, 0x00, 0x9D};
    std::string_view missingTextureName("missingTexture");
    GLuint missingTextureId;
    glGenTextures(1, &missingTextureId);
    glBindTexture(GL_TEXTURE_2D, missingTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, missingTexture);
    m_textures.emplace(missingTextureName, missingTextureId);
    
    // Implement the rest of the textures..
    for (auto textureName : textureList) {
        std::filesystem::path texturePath = std::filesystem::path("assets") / "textures" / textureName;
        GLuint textureId = loadTexture(texturePath.string());
        if (textureId) {
            m_textures.emplace(textureName, textureId);
        }
    }
}

GLuint TextureManager::loadTexture(const std::string_view &path) {
    SDL_Surface *textureSurface = IMG_Load(path.data());
    if (!textureSurface) {
        logger.log<ERROR>("Failed to load texture {}.", path);
        return 0;
    }
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureSurface->w, textureSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
    SDL_FreeSurface(textureSurface);
    return textureId;
}

