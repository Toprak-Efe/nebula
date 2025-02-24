#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include "coordinates.h"

typedef struct {
    mat4 projection;
    float arclength;
    equatorial_pose_t direction;
    cartesian_pose_t position;
} camera_t;

extern mat4 axis_remapping_matrix;

void camera_model_transform(camera_t *camera, mat4 out);

void camera_view_transform(camera_t *camera, mat4 out);

void camera_move(camera_t *camera, vec3 delta);

void camera_init(camera_t *camera);

#endif // CAMERA_H
