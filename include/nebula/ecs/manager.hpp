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

} // namespace nebula::ecs

