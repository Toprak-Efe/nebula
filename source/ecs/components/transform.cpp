#include <glm/ext/quaternion_float.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "../../../include/nebula/ecs/components/transform.hpp"
#include "../../../include/nebula/utils/math.hpp"
#include "../../../include/nebula/utils/conversion.hpp"

using namespace nebula::data;

Transform::Transform() :
    orientation{1.0f, 0.0f, 0.0f, 0.0f},
    position{0.0f},
    scale{1.0f, 1.0f, 1.0f}
{
    return;
} 

Mat4 Transform::getModelTransform() const {
    Mat4 out;
    glm::mat4 mat_model = glm::identity<glm::mat4>();
    glm::vec3 pose{position[0], position[1], position[2]};
    glm::quat quat{orientation[0], orientation[1], orientation[2], orientation[3]};
    glm::vec3 size{scale[0], scale[1], scale[2]};
    mat_model = glm::translate(mat_model, pose); 
    mat_model = mat_model*glm::mat4_cast(quat);
    mat_model = glm::scale(mat_model, size); 
    toNebula(mat_model, out);
    return out;
}

Mat4 Transform::getInverseModelTransform() const {
    Mat4 out;
    glm::mat4 mat_model;
    fromNebula(getModelTransform(), mat_model);
    glm::mat4 inverse = glm::inverse(mat_model);
    toNebula(inverse, out);
    return out;
}
