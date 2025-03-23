#include "coordinates.h"

void equatorial_to_cartesian(const equatorial_pose_t *equatorial, cartesian_pose_t *cartesian) {
    
    double r_proj = cos(glm_rad(equatorial->dec));
    cartesian->x = -equatorial->r * sin(glm_rad(equatorial->dec));;
    cartesian->y = -equatorial->r * r_proj * cos(glm_rad(equatorial->ra));
    cartesian->z = -equatorial->r * r_proj * sin(glm_rad(equatorial->ra));
}

void cartesian_to_equatorial(const cartesian_pose_t *cartesian, equatorial_pose_t *equatorial) {
    equatorial->r = sqrt(cartesian->x * cartesian->x + cartesian->y * cartesian->y + cartesian->z * cartesian->z);
    equatorial->ra = atan2(cartesian->x, cartesian->z); // cut-off region? maybe problem
    equatorial->dec = atan2(-cartesian->y, sqrt(cartesian->z * cartesian->z + cartesian->x * cartesian->x));
}