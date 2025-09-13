#ifndef ECS_HPP
#define ECS_HPP

#include <flecs.h>

namespace ecs {

class ECSManager {
public:
    ECSManager();
    ~ECSManager();
    void initialize();
private:
    flecs::world m_world;
};

inline ECSManager manager;

}

#endif//ECS_HPP
