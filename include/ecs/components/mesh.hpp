#ifndef MESH_HPP
#define MESH_HPP

#include <string_view>
#include <ecs/ecsmanager.hpp>
#include <glm/glm.hpp>

namespace nebula::data {

struct Mesh {
    std::string_view name;

    Mesh();
    Mesh(const std::string_view &name);
}; // struct Mesh

inline ECSComponentRegisterer<Mesh> meshRegisterer;

} // namespace nebula::data


#endif//MESH_HPP
