#pragma once

#include <glm/glm.hpp>
#include "math.hpp"

namespace nebula::data {

glm::mat4 nebulaToGlm(Mat4 mat_in); 

Mat4 glmToNebula(glm::mat4 mat_in); 

} // namespace nebula::data

