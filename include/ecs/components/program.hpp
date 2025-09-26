#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <string_view>
#include <ecs/ecsmanager.hpp>

namespace nebula::data {

struct Program {
    std::string_view name;

    Program();
    Program(const std::string_view &name);
}; // struct Program

inline ECSComponentRegisterer<Program> programRegisterer;

} // namespace nebula::data

#endif//PROGRAM_HPP
