#pragma once 

#include <string_view>

namespace nebula::data {

struct Program {
    std::string_view name;

    Program();
    Program(const std::string_view &name);
}; // struct Program

} // namespace nebula::data

