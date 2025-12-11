#include <algorithm>
#include <cassert>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <string_view>
#include <vector>
#include <regex>
#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL2/SDL_opengl.h>

#include "../../include/nebula/resources/shaders.hpp"
#include "../../include/nebula/utils/functions.hpp"
#include "../../include/nebula/utils/path.hpp"
#include "../../include/nebula/utils/log.hpp"

using namespace nebula::resources;

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
    return;
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

ShaderProgram::ShaderProgram(const GLuint id, std::map<std::string_view, GLint> &dict) :
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

void ShaderProgram::addUniform(const std::string_view &str, const GLint val) {
    udict[str] = val;
}

GLint ShaderProgram::getUniform(const std::string_view &str) const {
    assert(udict.contains(str));
    return udict.at(str);
}

ShaderManager::ShaderManager() {
    return;
}

std::optional<ShaderProgram> generateShader(const std::string_view &source, std::string name) {
    static const std::map<std::string_view, GLenum> shader_type_map{
        {"vertex",      GL_VERTEX_SHADER},
        {"geometry",    GL_GEOMETRY_SHADER},
        {"fragment",    GL_FRAGMENT_SHADER}
    }; 

    { /* Extract separate subshaders */
        std::vector <std::pair<GLenum, std::string>> shaders_parsed;
        shaders_parsed.reserve(3);
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
            shaders_parsed.emplace_back(std::pair{shader_type, shader_source}); 
        }
    }

    { /* Extract uniform variables */
        std::vector <std::pair <std::string, std::>> subshader_programs;
        std::vector <std::pair<GLenum, std::string>> shaders_parsed;
        shaders_parsed.reserve(3);
        const std::string_view key = "#shader";
        size_t cursor = source.find(key);
        key = "uniform";
        cursor = source.find()
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
        if (program.has_value()) {
            m_shader_programs.emplace(shader_desc.first, program.value());
        }
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

