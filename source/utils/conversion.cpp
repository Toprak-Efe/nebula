#include "../../include/nebula/utils/conversion.hpp"

glm::mat4 nebula::data::nebulaToGlm(nebula::data::Mat4 mat_in) {
    glm::mat4 out;
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            out[i][j] = mat_in[i][j];
        }
    }
    return out;
}

nebula::data::Mat4 nebula::data::glmToNebula(glm::mat4 mat_in) {
    nebula::data::Mat4 out;
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            out[i][j] = mat_in[i][j];
        }
    }
    return out;
} 

