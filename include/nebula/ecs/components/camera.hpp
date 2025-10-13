#pragma once 

#include <cstdint>
#include "../manager.hpp"
#include "../../utils/math.hpp"

namespace nebula::data {

struct Camera {
    uint16_t width;
    uint16_t height;
    float arclength;

    Camera();
    Camera(uint16_t width, uint16_t height, float arclength);
    Mat4 getProjectionMatrix() const;
}; // struct Camera

struct Active {};

inline ECSComponentRegisterer<Camera> cameraRegisterer;

} // namespace nebula::data

