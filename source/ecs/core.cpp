#include "../../include/nebula/ecs/core.hpp"
#include "../../include/nebula/ecs/components/camera.hpp"
#include "../../include/nebula/ecs/components/mesh.hpp"
#include "../../include/nebula/ecs/components/program.hpp"
#include "../../include/nebula/ecs/components/surface.hpp"
#include "../../include/nebula/ecs/components/transform.hpp"
#include "../../include/nebula/ecs/components/velocity.hpp"


nebula::data::core::core(flecs::world &world) {
    world.module<nebula::data::core>();
    world.component<nebula::data::Camera>();
    world.component<nebula::data::Mesh>();
    world.component<nebula::data::Program>();
    world.component<nebula::data::Surface>();
    world.component<nebula::data::Transform>();
    world.component<nebula::data::Velocity>();
}
