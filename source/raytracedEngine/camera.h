#pragma once

#include "utility.h"

class camera 
{
public:
    // Constructors
    camera() 
    {
        double aspect_ratio = 16.0 / 9.0;
        double viewport_height = 2.0;
        double viewport_width = aspect_ratio * viewport_height;
        // Distance of the canvas
        double focal_length = 1.0;

        origin = point3(0, 0, 0);
        horizontal = vec3(viewport_width, 0.0, 0.0);
        vertical = vec3(0.0, viewport_height, 0.0);
        lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
    }

    // Return a ray from the UV coordinates of the canvas
    // (lower_left_corner being the ray pointing to the canvas at (0.0, 0.0))
    ray get_ray(double u, double v) const 
    {
        return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
    }

private:
    // Position of the camera
    vec3 origin;
    // Ray from the origin to the lower_left_corner of the canvas
    vec3 lower_left_corner;
    // X axis
    vec3 horizontal;
    // Y axis
    vec3 vertical;
};
