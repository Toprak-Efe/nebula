#ifndef MESH_HPP
#define MESH_HPP

#include <string_view>
#include <ecs/ecsmanager.hpp>
#include <glm/glm.hpp>

namespace astronomy::data {

struct Mesh {
    std::string_view name;

    Mesh();
    Mesh(const std::string_view &name);
}; // struct Mesh

inline ECSComponentRegisterer<Mesh> meshRegisterer;

} // namespace astronomy::data


#endif//MESH_HPP
