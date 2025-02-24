#ifndef COORDINATES_h
#define COORDINATES_h

#include <stdint.h>
#include <GL/gl.h>
#include <math.h>

typedef struct {
    GLfloat x, y, z;
} cartesian_pose_t;

typedef struct {
    GLfloat ra, dec;
} equatorial_pose_t;

void equatorial_to_cartesian(const equatorial_pose_t *equatorial, cartesian_pose_t *cartesian);
void cartesian_to_equatorial(const cartesian_pose_t *cartesian, equatorial_pose_t *equatorial);

#endif // COORDINATES_h
