#include <ecs/components/velocity.hpp>

nebula::data::Velocity::Velocity() :
    radial(1.0, 0.0, 0.0, 0.0),
    positional(0.0, 0.0, 0.0)
{
    return;
}
