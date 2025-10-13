#pragma once 

#include <flecs.h>

namespace nebula::data {

class ECSManager {
public:
    ~ECSManager();
    ECSManager(const ECSManager&) = delete;
    ECSManager(ECSManager&&) = delete;
    ECSManager& operator=(const ECSManager&) = delete;
    static ECSManager& get();
    void initialize();
    void progressSystems(float d);
    void registerComponentType();
    flecs::world &getRegistry();
private:
    ECSManager();
    flecs::world m_world;
}; // class ECSManager

template<typename T>
struct ECSComponentRegisterer {
    ECSComponentRegisterer() {
        ECSManager &ecsManager = ECSManager::get();
        flecs::world &ecs = ecsManager.getRegistry(); 
        ecs.component<T>();
    }
}; // namespace ECSComponentRegisterer

template<typename T>
struct ECSSystemRegisterer {
    ECSSystemRegisterer(flecs::system &&sys) {
    }
}; // namespace ECSSystemRegisterer

} // namespace nebula::ecs

