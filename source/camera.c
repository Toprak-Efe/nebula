#include "camera.h"

void camera_model_transform(camera_t *camera, mat4 out) {
    mat4 m_rotation, m_transform;
    vec3 v_rotation = {0.0f, -glm_rad(camera->direction.dec), glm_rad(camera->direction.ra)};
    vec4 v_transform = {camera->position.x, camera->position.y, camera->position.z, 0.0f};
    glm_euler_xyz(v_rotation, m_rotation);
    glm_translate_make(m_transform, v_transform);
    glm_mat4_mul(m_transform, m_rotation, out);
}

void camera_view_transform(camera_t *camera, mat4 out) {
    mat4 model_transform;
    camera_model_transform(camera, model_transform);
    glm_mat4_inv(model_transform, out);
}

void camera_move(camera_t *camera, vec3 delta) {
    mat4 transform;
    camera_model_transform(camera, transform);
    vec4 delta_p = {delta[0], delta[1], delta[2], 0.0f};
    vec4 output = {0.0f, 0.0f, 0.0f, 1.0f};
    glm_mat4_mulv(transform, delta_p, output);
    camera->position.x += output[0];
    camera->position.y += output[1];
    camera->position.z += output[2];
}

void camera_raycast(camera_t *camera, equatorial_pose_t *pose, float x, float y) {
    int w, h;
    float x_norm = ((float) x / (float) w)*2.0 - 1.0;
    float y_norm = ((float) y / (float) h)*2.0 - 1.0;

    SDL_GetWindowSize(g_window->window, &w, &h);
    vec4 mouse_ray = {x_norm, y_norm, -1.0, 1.0};
    mat4 perspective_inv;
    glm_mat4_inv(camera->projection, perspective_inv);
    vec4 cursor_coords_v4;
    glm_mat4_mulv(perspective_inv, mouse_ray, cursor_coords_v4);
    vec3 cursor_coords = {cursor_coords_v4[0], cursor_coords_v4[1], cursor_coords_v4[2]};
    glm_vec3_normalize(cursor_coords);
    cartesian_pose_t camera_mouse_direction;
    camera_mouse_direction.x = cursor_coords[0];
    camera_mouse_direction.y = cursor_coords[1];
    camera_mouse_direction.z = cursor_coords[2];
    equatorial_pose_t cursor_direction;
    cartesian_to_equatorial(&camera_mouse_direction, pose);
}

void camera_forward(camera_t *camera, vec4 in, float *x, float *y) {
    mat4 view_transform;

    camera_view_transform(camera, view_transform);

    vec4 out;
    glm_mat4_mulv(view_transform, in, out);
    glm_mat4_mulv(camera->projection, out, out);

    *x = out[0];
    *y = out[1];
}

void camera_inverse(camera_t *camera, vec4 out, float x, float y) {
    mat4 v_inverse_transform;
    mat4 p_inverse_transform;
    mat4 view_transform;

    camera_view_transform(camera, view_transform);
    glm_mat4_inv(view_transform, v_inverse_transform);
    glm_mat4_inv(camera->projection, p_inverse_transform);

    vec4 p = {x, y, -0.5, 1.0};

    glm_mat4_mulv(p_inverse_transform, p, p);
    glm_mat4_mulv(v_inverse_transform, p, out);
}

void initialize_camera(camera_t *camera) {
    camera->arclength = 15.0f;
    glm_perspective(glm_rad(camera->arclength), 16.0f/9.0f, 0.01f, 1000.0f, camera->projection);
    camera->direction.r = 1.0f;
    camera->direction.ra = 0.0f;
    camera->direction.dec = 0.0f;
    camera->position.x = 0.0f;
    camera->position.y = 0.0f;
    camera->position.z = 0.0f;
}

void uninitialize_camera(camera_t *camera) {
    if (camera) {
        free(camera);
    }
}