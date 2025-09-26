#include <ecs/components/transform.hpp>

nebula::data::Transform::Transform() :
    orientation(1.0f, 0.0f, 0.0f, 0.0f),
    position(0.0),
    scale(1.0)
{
    return;
} 

glm::mat4 nebula::data::Transform::getModelTransform() const {
    glm::mat4 mat_model = glm::identity<glm::mat4>();
    mat_model = glm::scale(mat_model, scale); 
    mat_model = glm::mat4_cast(orientation)*mat_model;
    mat_model = glm::translate(mat_model, position); 
    return mat_model; 
}

glm::mat4 nebula::data::Transform::getInverseModelTransform() const {
    glm::mat4 mat_model = getModelTransform();
    return glm::inverse(mat_model);
}
