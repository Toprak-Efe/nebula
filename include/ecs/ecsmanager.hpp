#ifndef ECSMANAGER_HPP
#define ECSMANAGER_HPP

#include <flecs.h>
#include <log.hpp>

namespace nebula::data {

class ECSManager {
public:
    void initialize();
    void progressSystems(float d);
    void registerComponentType();
    const flecs::world &getRegistry();
    ~ECSManager();
private:
    flecs::world m_ecs;
}; // class ECSManager

inline ECSManager ecsManager; 

template<typename T>
struct ECSComponentRegisterer {
    ECSComponentRegisterer() {
        const flecs::world &ecs = ecsManager.getRegistry(); 
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
