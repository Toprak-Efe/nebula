#ifndef ECSMANAGER_HPP
#define ECSMANAGER_HPP

#include <flecs.h>
#include <log.hpp>

namespace nebula::data {

class ECSManager {
public:
    ~ECSManager();
    ECSManager(const ECSManager&) = delete;
    ECSManager& operator=(const ECSManager&) = delete;
    static ECSManager& get();
    void initialize();
    void progressSystems(float d);
    void registerComponentType();
    flecs::world &getRegistry();
    ecs_world_t* getRegistryPtr();
private:
    ECSManager();
    flecs::world m_world;
}; // class ECSManager

template<typename T>
struct ECSComponentRegisterer {
    ECSComponentRegisterer() {
        ECSManager &ecsManager = ECSManager::get();
        flecs::world &ecs = ecsManager.getRegistry(); 
        logger.log<INFO>("Registering component {}.", typeid(T).name());
        ecs.component<T>();
    }
}; // namespace ECSComponentRegisterer

template<typename T>
struct ECSSystemRegisterer {
    ECSSystemRegisterer(flecs::system &&sys) {
        logger.log<INFO>("Registering system {}.", typeid(T).name());
    }
}; // namespace ECSSystemRegisterer

} // namespace nebula::ecs

#endif//ECSMANAGER_HPP
