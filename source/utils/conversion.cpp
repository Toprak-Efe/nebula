#include "../../include/nebula/utils/conversion.hpp"

using namespace nebula::data;

glm::mat4 nebulaToGlm(Mat4 mat_in) {
    glm::mat4 out;
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            out[i][j] = mat_in[i][j];
        }
    }
    return out;
}

Mat4 glmToNebula(glm::mat4 mat_in) {
    Mat4 out;
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            out[i][j] = mat_in[i][j];
        }
    }
    return out;
} 

