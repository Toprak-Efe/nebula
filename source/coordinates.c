#include "coordinates.h"

void equatorial_to_cartesian(const equatorial_pose_t *equatorial, cartesian_pose_t *cartesian) {
    double r_proj = cos(equatorial->dec);
    cartesian->x = r_proj * cos(equatorial->ra);
    cartesian->y = r_proj * sin(equatorial->ra);
    cartesian->z = sin(equatorial->dec);
}

void cartesian_to_equatorial(const cartesian_pose_t *cartesian, equatorial_pose_t *equatorial) {
    equatorial->ra = atan2(cartesian->y, cartesian->x);
    equatorial->dec = atan2(cartesian->z, sqrt(cartesian->x * cartesian->x + cartesian->y * cartesian->y));
}
