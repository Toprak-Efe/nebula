#ifndef SHADER_HPP
#define SHADER_HPP

#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <array>
#include <list>
#include <string_view>
#include <utility>
#include <stdlib.h>
#include <string>
#include <map>
#include <memory>
#include <log.hpp>

namespace astronomy::shaders {

struct shader_t {
    GLenum type;
    std::string_view source;
}; // shader_t

class Shader {
public:
    Shader();
    Shader(Shader& other);
    Shader(Shader&& other);
    Shader(GLenum type, const std::string_view &src, const GLuint id);
    ~Shader();
    GLuint getId() const;
    using Ptr = std::shared_ptr<Shader>;
private:
    GLuint id;
    GLenum type;
    std::string_view source;
}; // Shader

class ShaderProgram {
public:
    ShaderProgram();
    ShaderProgram(const GLuint id);
    ShaderProgram(const GLuint id, std::map<std::string_view, GLint> &dict);
    ShaderProgram(ShaderProgram& other);
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

namespace resources {

inline constexpr auto shaderList = std::to_array({
    []() consteval {
        static constexpr char source[] = {
            #embed "../shaders/vertex.glsl"
        };
        return std::pair{
            std::string_view{"vertex"},
            shader_t{GL_VERTEX_SHADER, std::string_view{source, sizeof(source)}}
        };
    }(), 
    []() consteval {
        static constexpr char source[] = {
            #embed "../shaders/fragment.glsl"
        };
        return std::pair{
            std::string_view{"fragment"},
            shader_t{GL_FRAGMENT_SHADER, std::string_view{source, sizeof(source)}}
        };
    }(),
    []() consteval {
        static constexpr char source[] = {
            #embed "../shaders/geometry.glsl"
        };
        return std::pair{
            std::string_view{"geometry"},
            shader_t{GL_GEOMETRY_SHADER, std::string_view{source, sizeof(source)}}
        };
    }()
}); 

inline constexpr auto shaderProgramList = std::to_array({
    std::pair{
        std::string_view{"starShader"},
        std::pair {
            std::to_array<std::string_view>({ 
                "vertex",
                "fragment",
                "geometry"
            }),
            std::to_array<std::string_view>({
                "transformation",
                "color"
            })
        }
    },
});

}; // namespace resources

class ShaderManager {   
public:
    void initialize();
    ~ShaderManager();
private:
    std::map<std::string_view, ShaderProgram> m_shader_programs;
}; // ShaderManager

inline ShaderManager shaderManager;

}; // namespace astronomy::shaders

#endif // SHADER_HPP
