#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <cstdint>
#include <glm/glm.hpp>
#include <ecs/ecsmanager.hpp>

namespace astronomy::data {

struct Camera {
    uint16_t width;
    uint16_t height;
    float arclength;

    Camera();
    Camera(uint16_t width, uint16_t height, float arclength);
    glm::mat4 getProjectionMatrix() const;
}; // struct Camera

struct Active {};

inline ECSComponentRegisterer<Camera> cameraRegisterer;

} // namespace astronomy::data

#endif//CAMERA_HPP
