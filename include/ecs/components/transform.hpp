#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <ecs/ecsmanager.hpp>

namespace nebula::data {

struct Transform {
    glm::quat orientation;
    glm::vec3 position;
    glm::vec3 scale;

    Transform();
    glm::mat4 getModelTransform() const;
    glm::mat4 getInverseModelTransform() const;

}; // struct Transform

inline ECSComponentRegisterer<Transform> transformRegisterer;

} // namespace nebula::data

#endif//TRANSFORM_HPP
