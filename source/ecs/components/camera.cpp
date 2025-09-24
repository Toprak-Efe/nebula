#include "glm/ext/matrix_clip_space.hpp"
#include <ecs/components/camera.hpp>

astronomy::data::Camera::Camera() :
    width(1920),
    height(1080),
    arclength(60.0)
{
    return;
}

astronomy::data::Camera::Camera(uint16_t width, uint16_t height, float arclength) :
    width(width),
    height(height),
    arclength(arclength)
{
    return;
}

glm::mat4 astronomy::data::Camera::getProjectionMatrix() const {
    return glm::perspective(arclength, (float) width / (float) height, 0.01f, 10000.0f);
}
