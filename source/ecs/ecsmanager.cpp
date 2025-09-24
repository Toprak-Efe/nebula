#include <ecs/ecsmanager.hpp>

using namespace astronomy::data;
    

void ECSManager::initialize() {
    
}

void ECSManager::progressSystems(float d) {
    m_ecs.progress(d);
}

void ECSManager::registerComponentType() {

}

const flecs::world &ECSManager::getRegistry() {
    return m_ecs;
}

ECSManager::~ECSManager() {

}
