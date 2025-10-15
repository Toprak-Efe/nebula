#pragma once

#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "math.hpp"

namespace nebula::data {

inline
void toNebula(const glm::mat4 &glm_mat, Mat4 &nebula_mat) {
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            nebula_mat[i][j] = glm_mat[i][j];
        }
    }
} 

inline
void toNebula(const glm::mat3 &glm_mat, Mat3 &nebula_mat) {
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            nebula_mat[i][j] = glm_mat[i][j];
        }
    }
} 

inline
void toNebula(const glm::vec3 &glm_vec, Vec3 &nebula_vec) {
    for (size_t i = 0; i < 3; i++) {
            nebula_vec[i] = glm_vec[i];
    }
} 

inline
void toNebula(const glm::vec4 &glm_vec, Vec4 &nebula_vec) {
    for (size_t i = 0; i < 4; i++) {
        nebula_vec[i] = glm_vec[i];
    }
} 

inline
void toNebula(const glm::quat &glm_quat, Vec4 &nebula_vec) {
    for (size_t i = 0; i < 4; i++) {
        nebula_vec[i] = glm_quat[i];
    }
} 


inline
void fromNebula(const Mat4 &nebula_mat, glm::mat4 &glm_mat) {
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            glm_mat[i][j] = nebula_mat[i][j];
        }
    }
} 

inline
void fromNebula(const Mat3 &nebula_mat, glm::mat3 &glm_mat) {
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            glm_mat[i][j] = nebula_mat[i][j];
        }
    }
} 

inline
void fromNebula(const Vec3 &nebula_vec, glm::vec3 &glm_vec) {
    for (size_t i = 0; i < 3; i++) {
        glm_vec[i] = nebula_vec[i];
    }
} 

inline
void fromNebula(const Vec4 &nebula_vec, glm::vec4 &glm_vec) {
    for (size_t i = 0; i < 4; i++) {
        glm_vec[i] = nebula_vec[i];
    }
} 

inline
void fromNebula(const Vec4 &nebula_vec, glm::quat &glm_quat) {
    for (size_t i = 0; i < 4; i++) {
        glm_quat[i] = nebula_vec[i];
    }
} 

} // namespace nebula::data

