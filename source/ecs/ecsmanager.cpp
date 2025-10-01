#include <ecs/ecsmanager.hpp>

using namespace nebula::data;
    
ECSManager::ECSManager() :
    m_world()
{

}

ECSManager& ECSManager::get() {
    static ECSManager instance;
    return instance;
}

ECSManager::~ECSManager() {

}

void ECSManager::initialize() {
    m_world.set<flecs::Rest>({});
    m_world.import<flecs::stats>();
}

void ECSManager::progressSystems(float d) {
    m_world.progress(d);
}

void ECSManager::registerComponentType() {

}

flecs::world &ECSManager::getRegistry() {
    return m_world;
}

ecs_world_t* ECSManager::getRegistryPtr() {
    return m_world.c_ptr();
}

