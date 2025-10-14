#include "../../include/nebula/ecs/manager.hpp"
#include "../../include/nebula/ecs/core.hpp"

nebula::data::ECSManager::ECSManager() :
    m_world()
{
    m_world.import<nebula::data::core>();
}

nebula::data::ECSManager& nebula::data::ECSManager::get() {
    static nebula::data::ECSManager instance;
    return instance;
}

nebula::data::ECSManager::~ECSManager() {

}

void nebula::data::ECSManager::initialize() {
    m_world.set<flecs::Rest>({});
    m_world.import<flecs::stats>();
}

void nebula::data::ECSManager::progressSystems(float d) {
    m_world.progress(d);
}

void nebula::data::ECSManager::registerComponentType() {

}

flecs::world &nebula::data::ECSManager::getRegistry() {
    return m_world;
}


