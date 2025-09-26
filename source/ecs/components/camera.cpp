#include "glm/ext/matrix_clip_space.hpp"
#include <ecs/components/camera.hpp>

nebula::data::Camera::Camera() :
    width(1920),
    height(1080),
    arclength(90.0f)
{
    return;
}

nebula::data::Camera::Camera(uint16_t width, uint16_t height, float arclength) :
    width(width),
    height(height),
    arclength(arclength)
{
    return;
}

glm::mat4 nebula::data::Camera::getProjectionMatrix() const {
    float fov = arclength*std::numbers::pi_v<float>/180.0;
    return glm::perspective(fov, (float) width / (float) height, 0.01f, 10000.0f);
}
