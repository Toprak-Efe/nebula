#pragma once

#include "../../utils/math.hpp"

namespace nebula::data {

struct Velocity {
    Vec4 radial{0.0f};
    Vec3 positional{0.0f};

    Velocity();
}; // struct Velocity

} // namespace nebula::data

