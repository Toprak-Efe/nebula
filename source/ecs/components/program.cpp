#include "../../../include/nebula/ecs/components/program.hpp"

nebula::data::Program::Program()
{
    return;
}

nebula::data::Program::Program(const std::string_view &name) : name(name)
{
    return;
}
