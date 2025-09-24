#include "SDL_opengl.h"
#include "functions.h"
#include "log.hpp"
#include <GL/glext.h>
#include <cassert>
#include <shaders.hpp>

using namespace astronomy::resources;

Shader::Shader(Shader&& other) :
    id(other.id),
    type(other.type),
    source(other.source)
{
    other.id = 0;
}

Shader::Shader() :
    id(),
    type(),
    source()
{
    return;
}

Shader::Shader(GLenum type, const std::string_view &src, const GLuint id) :
    id(id),
    type(type),
    source(src)
{
    return;
}

Shader::~Shader() {
    if (id) {
        glDeleteShader(id);
    }
}

GLuint Shader::getId() const {
    return this->id; 
}

ShaderProgram::ShaderProgram() :
    id(0),
    udict()
{
    return;
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) :
    id(other.id),
    udict(std::move(other.udict))
{
    other.id = 0;
}

ShaderProgram::ShaderProgram(const GLuint id) :
    id(id),
    udict()
{
    return;
}

ShaderProgram::ShaderProgram(const GLuint id, std::map<std::string_view, GLint> &dict) :
    id(id),
    udict(std::move(dict))
{
    return;
}

ShaderProgram::~ShaderProgram() 
{
    if (id) {
        glDeleteProgram(id);
    }
}

GLuint ShaderProgram::getId() const {
    return this->id; 
}

void ShaderProgram::addUniform(const std::string_view &str, const GLint val) {
    udict[str] = val;
}

GLint ShaderProgram::getUniform(const std::string_view &str) const {
    assert(udict.contains(str));
    return udict.at(str);
}

void ShaderManager::initialize() {
    std::map<std::string_view, Shader> shaders;

    static std::vector<std::pair<std::string_view, shader_t>> shaderList = {
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
                std::string_view{"vertex"},
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
        }(),
    };

    static std::vector<std::pair<std::string_view, std::pair<std::vector<std::string_view>, std::vector<std::string_view>>>> shaderProgramList = {
        std::pair{
            std::string_view{"starShader"},
            std::pair {
                std::vector<std::string_view>({ 
                    "vertex",
                    "fragment",
                    "geometry"
                }),
                std::vector<std::string_view>({
                    "transformation",
                    "color"
                })
            }
        }
    };

    logger.log<INFO>("Building shaders.");
    for (auto &shader : shaderList) {
        const std::string_view &name = shader.first;
        const GLenum type = shader.second.type; 

        const std::string_view &source = shader.second.source;
        GLint source_size = source.length(); 

        GLuint gs = glCreateShader(type);
        const char *sourcePtr = source.data();
        glShaderSource(gs, 1, (const GLchar **) &sourcePtr, &source_size);
        glCompileShader(gs);

        GLint success;
        glGetShaderiv(gs, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info_log[256];
            glGetShaderInfoLog(gs, sizeof(info_log), NULL, info_log);
            logger.log<INFO>("Shader {} failed to compile. {}", name, info_log);
        }

        shaders.emplace(name, Shader(type, source, gs));
    }

    logger.log<INFO>("Linking shaders.");
    for (auto &shaderProgram : shaderProgramList) {
        GLuint shaderProgramId = glCreateProgram();
        
        for (auto &shader : shaderProgram.second.first) {
            glAttachShader(shaderProgramId, shaders[shader].getId());
        } 
        
        glLinkProgram(shaderProgramId);
        
        GLint success;
        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
        if (!success) {
            char info_log[256];
            glGetProgramInfoLog(shaderProgramId, sizeof(info_log), NULL, info_log);
            logger.log<INFO>("Shader {} failed to compile. {}", shaderProgram.first, info_log);
        } else {
            logger.log<INFO>("Shader {} successfully compiled.", shaderProgram.first);
        }
        
        m_shader_programs.emplace(shaderProgram.first, ShaderProgram(shaderProgramId));
        for (auto &uniformName : shaderProgram.second.second) {
            GLint uId = glGetUniformLocation(shaderProgramId, uniformName.data());
            m_shader_programs[shaderProgram.first].addUniform(uniformName, uId);
        }
    }

    for (const auto& [name, shader] : shaders) {
        glDeleteShader(shader.getId());
    }

}

ShaderManager::~ShaderManager() {
    for (auto &[name, shader]: m_shader_programs) {
        glDeleteProgram(shader.getId());
    }
}

const ShaderProgram &ShaderManager::getProgram(const std::string_view program_name) const {
    assert(m_shader_programs.contains(program_name));
    return m_shader_programs.at(program_name);
}

