#include <algorithm>
#include <numeric>
#include <optional>
#include <vector>
#include <cassert>
#include <cstring>
#include <cstdint>
#include <filesystem>
#include <ios>

#include "../../include/nebula/utils/log.hpp"
#include "../../include/nebula/resources/meshes.hpp"
#include "../../include/nebula/utils/functions.hpp"
#include "../../include/nebula/utils/path.hpp"

using namespace nebula::resources;

#pragma pack(push, 1) 
struct StlTri {
    float normal[3];
    float vertices[3][3];
    uint16_t attribute_count;
};
#pragma pack(pop) 

struct GlVertex {
    float position[3];
    float normal[3];
};

std::optional<Mesh> parse_stl(const std::filesystem::path &path) {
    if (path.empty() || !std::filesystem::exists(path) || std::filesystem::is_directory(path) || path.extension() != "stl") {
        return {}; // unacceptable inode 
    }
    
    size_t length = std::filesystem::file_size(path);
    if (length < 84) return {};
   
    std::ifstream stl_file(path, std::ios::binary);
    stl_file.ignore(80);
    
    uint32_t number_of_triangles = 0; 
    stl_file.read(reinterpret_cast<char*> (&number_of_triangles), 4);

    if (length != number_of_triangles*50 + 84) {
        return {}; // filesize mismatch
    }
    
    std::vector<StlTri> triangle_entries(number_of_triangles);
    stl_file.read(reinterpret_cast<char*>(triangle_entries.data()), number_of_triangles * sizeof(StlTri));

    std::vector<GlVertex> gl_vertices;
    gl_vertices.reserve(number_of_triangles * 3);
    
    std::ranges::for_each(triangle_entries, [&](const auto& tri){
        gl_vertices.push_back({ {tri.vertices[0][0], tri.vertices[0][1], tri.vertices[0][2]}, {tri.normal[0], tri.normal[1], tri.normal[2]} });
        gl_vertices.push_back({ {tri.vertices[1][0], tri.vertices[1][1], tri.vertices[1][2]}, {tri.normal[0], tri.normal[1], tri.normal[2]} });
        gl_vertices.push_back({ {tri.vertices[2][0], tri.vertices[2][1], tri.vertices[2][2]}, {tri.normal[0], tri.normal[1], tri.normal[2]} });
    });
        
    std::array<GLuint, 2> vo; // vo[0]=VBO, vo[1]=VAO
    glGenVertexArrays(1, &vo[1]);
    glGenBuffers(1, &vo[0]);

    glBindVertexArray(vo[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vo[0]);

    glBufferData(GL_ARRAY_BUFFER, gl_vertices.size() * sizeof(GlVertex), gl_vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GlVertex), (void*)offsetof(GlVertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GlVertex), (void*)offsetof(GlVertex, normal));

    glBindVertexArray(0);
    
    return Mesh(vo[0], vo[1], gl_vertices.size());
}

Mesh default_triangle() {
    const GLfloat triangle_vertices[] = {
        // positions         // normals (unused but keeps stride consistent)
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
    };

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);

    return Mesh{vbo, vao, 3};
}

Mesh default_cube() {
    const GLfloat cube_vertices[] = {
        // positions           // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);

    return Mesh{vbo, vao, 36};
}

MeshManager::MeshManager() {
    return;
}

void MeshManager::initialize() {
    m_meshes.emplace("cube", default_cube());
    m_meshes.emplace("triangle", default_triangle());
    std::filesystem::path meshes = nebula::get_asset_dir() / "meshes"; 
    if (!std::filesystem::is_directory(meshes)) return;
    for (auto const &file_entry : std::filesystem::directory_iterator(meshes)) {
        if (!file_entry.is_regular_file()) return;
        std::string extension = file_entry.path().extension().string();
        std::optional <Mesh> parse_result; 
        if (extension == "stl") {
            parse_result = parse_stl(file_entry.path());
        } else { // TO-DO: Add more parsers 
            continue; 
        }
        if (parse_result.has_value()) {
            std::string name = file_entry.path().filename();
            Mesh mesh = parse_result.value();
            m_meshes[name] = mesh;
        }
    }
}

MeshManager::~MeshManager() {
    for (auto &[name, mesh]: m_meshes) {
        glDeleteBuffers(1, &mesh.vbo);
        glDeleteVertexArrays(1, &mesh.vao);
    }
}

MeshManager &MeshManager::get() {
    static MeshManager meshManager;
    return meshManager;
} 

const Mesh &MeshManager::getMesh(const std::string_view &mesh_name) const {
    assert(m_meshes.contains(mesh_name));
    return m_meshes.at(mesh_name); 
}
