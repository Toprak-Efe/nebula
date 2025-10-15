#pragma once 

#include <string_view>

namespace nebula::data {

struct Mesh {
    std::string_view name;

    Mesh();
    Mesh(const std::string_view &name);
}; // struct Mesh

} // namespace nebula::data

