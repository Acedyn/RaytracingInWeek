#pragma once

#include "utility.h"

class camera 
{
public:
    // Constructors
    camera(vec3 lookfrom, 
        vec3 lookat, 
        vec3 vup, 
        double vfov, 
        double aspect_ratio,
        double aperture,
        double focus_dist,
        double _time0 = 0,
        double _time1 = 0) 
    {
        double theta = degrees_to_radians(vfov);
        double h = tan(theta/2);
        double viewport_height = 2.0 * h;
        double viewport_width = aspect_ratio * viewport_height;
        // Distance of the canvas
        double focal_length = 1.0;

        // X axis
        w = unit_vector(lookfrom - lookat);
        // Z axis
        u = unit_vector(cross(vup, w));
        // Y axis
        v = cross(w, u);

        // Position
        origin = lookfrom;
        // Horizontal size of the canvas
        horizontal = focus_dist * viewport_width * u;
        // Vertical size of the canvas
        vertical = focus_dist * viewport_height * v;
        // Vector pointing from the camera to the lower left corner of the canvas
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

        // For defocus
        lens_radius = aperture / 2;
        time0 = _time0;
        time1 = _time1;
    }

    // Return a ray from the UV coordinates of the canvas
    // (lower_left_corner being the ray pointing to the canvas at (0.0, 0.0))
    ray get_ray(double s, double t) const 
    {
        // We offset the origin of the ray randomly according to the lens_radius
        // (biger the lens_radius is, bigger the randomization is)
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        // We return a ray from offseted origin at a random time between the two time samples
        return ray(
            origin + offset,
            lower_left_corner + s*horizontal + t*vertical - origin - offset,
            random_double(time0, time1)
        );
    }

private:
    // Position of the camera
    vec3 origin;
    // Ray from the origin to the lower_left_corner of the canvas
    vec3 lower_left_corner;
    // U canvas's axis
    vec3 horizontal;
    // V canvas's axis
    vec3 vertical;
    // X, Y, Z axis of the camera
    vec3 u, v, w;
    // For defocus
    double lens_radius;
    double time0, time1;  // shutter open/close times
};
