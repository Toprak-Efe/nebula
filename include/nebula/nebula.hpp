#ifndef NEBULA_HPP
#define NEBULA_HPP

#include "ecs/manager.hpp"
#include "events/manager.hpp"

namespace nebula {

struct NebulaApi {
    events::EventManager *events;
    data::ECSManager *ecs;
}; // NebulaApi

namespace game {

/**
 * Define this function to initialize game.
 */
void initialize(NebulaApi &api);

}; // namespace game

}; // namespace nebula

#endif//NEBULA_HPP

