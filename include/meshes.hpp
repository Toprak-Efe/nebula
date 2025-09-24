#ifndef MESHES_HPP
#define MESHES_HPP

#include <map>
#include <cassert>
#include <string_view>
#include <GL/gl.h>
#include <log.hpp>
#include <functions.h>
#include <glm/glm.hpp>

namespace astronomy::resources {

struct Mesh {
    GLuint vbo, vao; 
    GLsizei vertex_count;
}; // struct mesh_t

class MeshManager { 
public:
    void initialize();
    ~MeshManager();
    const Mesh &getMesh(const std::string_view &mesh_name) const;
private:
    std::map<std::string_view, Mesh> m_meshes;
}; // class MeshManager

inline MeshManager meshManager;

}; // namespace astronomy::resources

#endif//MESHES_HPP
