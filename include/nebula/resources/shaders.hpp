#pragma once 

#include <GL/gl.h>
#include <string_view>
#include <memory>
#include <map>

namespace nebula::resources {

struct shader_t {
    GLenum type;
    std::string_view source;
}; // shader_t

class ShaderProgram {
public:
    ShaderProgram();
    ShaderProgram(const GLuint id);
    ShaderProgram(const GLuint id, std::map<std::string_view, GLint> &dict);
    ShaderProgram(const ShaderProgram& other);
    ShaderProgram(ShaderProgram&& other);
    ~ShaderProgram();
    GLuint getId() const;
    void addUniform(const std::string_view &str, const GLint val);
    GLint getUniform(const std::string_view &str) const;
    using Ptr = std::shared_ptr<ShaderProgram>;
private:
    GLuint id;
    std::map<std::string_view, GLint> udict;
}; // ShaderProgram

class ShaderManager {   
public:
    static ShaderManager &get();
    void initialize();
    ~ShaderManager();
    ShaderManager(const ShaderManager &) = delete;
    ShaderManager(ShaderManager &&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
    const ShaderProgram &getProgram(const std::string) const;
private:
    ShaderManager();
    std::map<std::string, ShaderProgram> m_shader_programs;
}; // ShaderManager

}; // namespace nebula::resources
   
