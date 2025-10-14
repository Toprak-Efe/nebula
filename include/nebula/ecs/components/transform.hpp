#pragma once

#include "../manager.hpp"
#include "../../utils/math.hpp"

namespace nebula::data {

struct Transform {
    Vec4 orientation;
    Vec3 position;
    Vec3 scale;

    Transform();
    Mat4 getModelTransform() const;
    Mat4 getInverseModelTransform() const;

}; // struct Transform

} // namespace nebula::data

