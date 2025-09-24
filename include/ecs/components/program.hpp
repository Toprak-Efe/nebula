#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <string_view>
#include <ecs/ecsmanager.hpp>

namespace astronomy::data {

struct Program {
    std::string_view name;

    Program();
    Program(const std::string_view &name);
}; // struct Program

inline ECSComponentRegisterer<Program> programRegisterer;

} // namespace astronomy::data

#endif//PROGRAM_HPP
