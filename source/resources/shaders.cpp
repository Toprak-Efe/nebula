#include <algorithm>
#include <cassert>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <fstream>
#include <map>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL2/SDL_opengl.h>

#include "../../include/nebula/resources/shaders.hpp"
#include "../../include/nebula/utils/functions.hpp"
#include "../../include/nebula/utils/path.hpp"
#include "../../include/nebula/utils/log.hpp"

using namespace nebula::resources;

ShaderProgram::ShaderProgram() :
    id(0),
    udict()
{
    return;
}

ShaderProgram::ShaderProgram(const ShaderProgram& other) :
    id(other.id),
    udict(other.udict)
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

ShaderProgram::ShaderProgram(const GLuint id, std::map<std::string, GLint> &dict) :
    id(id),
    udict(std::move(dict))
{
    return;
}

ShaderProgram::~ShaderProgram() 
{
    return;
}

GLuint ShaderProgram::getId() const {
    return this->id; 
}

void ShaderProgram::addUniform(const std::string &str, const GLint val) {
    udict[str] = val;
}

GLint ShaderProgram::getUniform(const std::string &str) const {
    assert(udict.contains(str));
    return udict.at(str);
}

ShaderManager::ShaderManager() {
    return;
}

std::optional<ShaderProgram> generateShader(const std::string_view &source, std::string name) {
    using namespace std::literals;

    static const std::array<std::pair<std::string_view, ShaderUniformType>, 4> supported_uniform_types{{
        {"float"sv, FLOAT},
        {"int"sv, INTEGER},
        {"vec3"sv, VEC3},
        {"mat4"sv, MAT4}
    }};
    static const std::map<std::string_view, GLenum> shader_type_map{
        {"vertex",      GL_VERTEX_SHADER},
        {"geometry",    GL_GEOMETRY_SHADER},
        {"fragment",    GL_FRAGMENT_SHADER}
    }; 
    nebula::Logger &logger = nebula::Logger::get();
    
    std::map<std::string, ShaderUniformType> uniforms;
    std::vector <std::pair<GLenum, std::string>> subshaders;
    subshaders.reserve(3);

    try { /* Extract separate subshaders */
        const std::string_view key = "#shader";
        size_t cursor = source.find(key);
        while (cursor != std::string_view::npos) {
            size_t type_begin = source.find_first_not_of(" \t", cursor + key.size());
            size_t type_end = source.find_first_of(" \t", type_begin); 
            std::string_view type_sv = source.substr(type_begin, type_end);

            GLenum shader_type;
            if (shader_type_map.contains(type_sv)) {
                shader_type = shader_type_map.at(type_sv);
            }

            size_t cursor = source.find(key, type_end);
            std::string_view shader_source = source.substr(type_end, cursor);
            subshaders.emplace_back(std::pair{shader_type, shader_source}); 
        }
        bool vertex{false}, fragment{false};
        for (const auto &[type, source] : subshaders) {
            if (type == GL_VERTEX_SHADER) vertex = true;
            if (type == GL_FRAGMENT_SHADER) fragment = true;
            if (!(vertex && fragment)) {
                logger.log<nebula::Levels::INFO>("Unable to find both a vertex and a fragment shader {}.", name);
                return {};
            }
        }
    } catch (std::exception &e) {
        logger.log<nebula::Levels::INFO>("Unable to parse subshader separations for {}: {}", name, e.what());
        return {};
    }
    
    for (const auto &[type, subsource] : subshaders) { /* Extract uniform variables */
        try {
            std::string_view uniform_declaration{"uniform"};
            std::size_t cursor;
            while ((cursor = subsource.find(uniform_declaration) != std::string_view::npos)) { // Find uniform type
                size_t u_type_begin = subsource.find_first_not_of(" \t", cursor + uniform_declaration.size());
                size_t u_type_end = subsource.find_first_of(" \t", u_type_begin);
                std::string u_type_sv = subsource.substr(u_type_begin, u_type_end);

                size_t u_name_begin = subsource.find_first_not_of(" \t", u_type_end);
                size_t u_name_end = subsource.find_first_of(" \t", u_name_begin);
                std::string u_name_sv = subsource.substr(u_name_begin, u_name_end);

                ShaderUniformType uniform_type;
                for (const auto &[type_name, type_enum] : supported_uniform_types) {
                    if (type_name == u_type_sv) uniforms.emplace(u_name_sv, type_enum);
                }
            }
        } catch (std::exception &e) {
            logger.log<nebula::Levels::INFO>("Unable to parse uniform field data for {}: {}", name, e.what());
            return {};
        }
    } 
    
    std::vector<GLuint> shaders;
    for (const auto &[subtype, subsource] : subshaders) { /* Compile subshaders */
        GLuint gs = glCreateShader(subtype);
        GLint subsource_size = subsource.length();
        const char *subsourceptr = subsource.data();
        glShaderSource(gs, 1, (const GLchar **) &subsourceptr, &subsource_size);
        glCompileShader(gs);
        GLint success;
        glGetShaderiv(gs, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info_log[256];
            glGetShaderInfoLog(gs, sizeof(info_log), NULL, info_log);
            logger.log<nebula::Levels::INFO>("Unable to compile {}: {}", name, info_log);
            return {};
        }
        shaders.push_back(gs);
    }

    { /* Link the shader program */
        // std::vector <std::pair<GLenum, std::string>> subshaders;
        // std::map<std::string, ShaderUniformType> uniforms;
        // std::vector<GLuint> shaders;

        GLuint shader_program_id = glCreateProgram();
        for (auto &shader : shaders) {
            glAttachShader(shader_program_id, shader); 
        } 
        glLinkProgram(shader_program_id);

        GLint success;
        glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
        if (!success) {
            char info_log[256];
            glGetProgramInfoLog(shader_program_id, sizeof(info_log), NULL, info_log);
            logger.log<nebula::Levels::INFO>("Failed to compile {}: {}", name, info_log);
        } else {
            logger.log<nebula::Levels::INFO>("Successfully compiled {}.", name);
        }

        ShaderProgram program{shader_program_id};
        for (const auto &[u_name, u_type] : uniforms) {
            GLint u_id = glGetUniformLocation(shader_program_id, u_name.data());
            program.addUniform(u_name, u_id);
        }
        return program;
    }

    return {};
};

void ShaderManager::initialize() {
    Logger &logger = Logger::get();
    std::vector<std::pair<std::string, std::string>> shaders_found;
    
    const std::string default_source = {
        #embed "../../shaders/default.glsl"
    };
    const std::string default_name = "default";
    shaders_found.emplace_back(default_name, default_source); 
    
    std::filesystem::path shaders_path = get_asset_dir() / "shaders";
    for (const auto &shader_iter : std::filesystem::directory_iterator(shaders_path)) {
        if (!shader_iter.is_regular_file() || shader_iter.path().extension() != "glsl") continue;
        logger.log<Levels::INFO>("Found Shader: %s", shader_iter.path().stem().string());
        
        std::string shader_name = shader_iter.path().stem();
        size_t shader_source_size = std::filesystem::file_size(shader_iter.path()); 
        std::string shader_source(shader_source_size, '\0'); 

        std::ifstream shader_file(shader_iter.path());
        shader_file.read(&shader_source[0], shader_source_size); 
        shaders_found.emplace_back(shader_name, shader_source);
    }

    std::ranges::for_each(shaders_found, [&](const auto& shader_desc){
        logger.log<Levels::INFO>("Compiling Shader: %s", shader_desc.first);
        std::optional<ShaderProgram> program = generateShader(shader_desc.first, shader_desc.second);
        if (program.has_value()) m_shader_programs.emplace(shader_desc.first, program.value());
    });
}

ShaderManager::~ShaderManager() {
    for (auto &[name, shader]: m_shader_programs) {
        glDeleteProgram(shader.getId());
    }
}

const ShaderProgram &ShaderManager::getProgram(const std::string program_name) const {
    assert(m_shader_programs.contains(program_name));
    return m_shader_programs.at(program_name);
}

ShaderManager &ShaderManager::get() {
    static ShaderManager shaderManager;
    return shaderManager;
}

