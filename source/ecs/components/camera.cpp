#include <numbers>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "../../../include/nebula/ecs/components/camera.hpp"
#include "../../../include/nebula/utils/math.hpp"
#include "../../../include/nebula/utils/conversion.hpp"

using namespace nebula::data;

Camera::Camera() :
    width(1920),
    height(1080),
    arclength(90.0f)
{
    return;
}

Camera::Camera(uint16_t width, uint16_t height, float arclength) :
    width(width),
    height(height),
    arclength(arclength)
{
    return;
}

Mat4 Camera::getProjectionMatrix() const {
    Mat4 out;
    float fov = arclength*std::numbers::pi_v<float>/180.0;
    glm::mat4 pers = glm::perspective(fov, (float) width / (float) height, 0.01f, 10000.0f);
    toNebula(pers, out);
    return out;
}
