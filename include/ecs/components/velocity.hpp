#ifndef VELOCITY_HPP
#define VELOCITY_HPP

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <ecs/ecsmanager.hpp>

namespace nebula::data {

struct Velocity {
    glm::vec4 radial{0.0f};
    glm::vec3 positional{0.0f};

    Velocity();
}; // struct Velocity

inline ECSComponentRegisterer<Velocity> velocityRegisterer;

} // namespace nebula::data

#endif//VELOCITY_HPP
