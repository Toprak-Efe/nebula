#pragma once 

#include <map>
#include <string_view>
#include <GL/gl.h>

namespace nebula::resources {

struct Mesh {
    GLuint vbo, vao; 
    GLsizei vertex_count;
}; // struct mesh_t

class MeshManager { 
public:
    void initialize();
    ~MeshManager();
    MeshManager(const MeshManager &) = delete;
    MeshManager(MeshManager &&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;
    static MeshManager &get();
    const Mesh &getMesh(const std::string_view &mesh_name) const;
private:
    MeshManager();
    std::map<std::string_view, Mesh> m_meshes;
}; // class MeshManager

}; // namespace nebula::resources

