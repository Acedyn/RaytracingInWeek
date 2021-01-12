#include "utility.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>
#include <fstream>

// Compute the hit location with a shpere
double hit_sphere(const vec3& sphereCenter, double sphereRadius, const ray& r) {
    vec3 oc = r.origin() - sphereCenter;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - sphereRadius * sphereRadius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) 
    {
        return -1.0;
    } 
    else 
    {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

// Get the hit color, if no hit, return the background (like a default color)
color ray_color(const ray& r, const hittable& world)
{
    hit_record rec;
    // Test if the ray hit one of the objects
    if (world.hit(r, 0, INFINITE, rec)) {
        // If we hit return the color at the location
        return 0.5 * (rec.normal + color(1,1,1));
    }
    // If no hit we return the background
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main()
{
    // Image size
    const double ASPECT_RATIO = 16.0f / 9.0f;
    const int IMG_WIDTH = 400;
    const int IMG_HEIGHT = static_cast<int>(IMG_WIDTH / ASPECT_RATIO);

    // World assets
    hittable_list world;
    world.add(std::make_shared<sphere>(vec3(0,0,-1), 0.5));
    world.add(std::make_shared<sphere>(vec3(0,-100.5,-1), 100));

    ////////////////////////////////////////
    // Camera
    ////////////////////////////////////////

    // Viewport
    double viewport_height = 2.0f;
    double viewport_width = ASPECT_RATIO * viewport_height;

    // Distance between the camera and the viewport
    double focal_length = 1.0;

    // Position of the camera
    vec3 origin = vec3(0.0f, 0.0f, 0.0f);
    // X axis of the camera's tranform
    vec3 horizontal = vec3(viewport_width, 0.0f, 0.0f);
    // Y acis of the camera's tranform
    vec3 vertical = vec3(0.0f, viewport_height, 0.0f);
    // Vector pointing from the camera's origin to the lower left corner of the viewport
    vec3 lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    // Output file
    std::ofstream file("raytracing.ppm");

    ////////////////////////////////////////
    // Rendering
    ////////////////////////////////////////
    file << "P3\n" << IMG_WIDTH << " " << IMG_HEIGHT << "\n255\n";

    // Loop over all the pixels
    for(int j = IMG_HEIGHT - 1; j >= 0; j--)
    {
        for(int i = 0; i < IMG_WIDTH; i++)
        {
            // Compute normalized coordinates
            double u = double(i) / (IMG_WIDTH-1);
            double v = double(j) / (IMG_HEIGHT-1);
            // Compute the ray from the camera to the current pixel on the viewport
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            // Test the ray hit over all the objects of the scene
            color pixel_color = ray_color(r, world);
            // Store the color
            write_color(file, pixel_color);
        }
    }

    // Output the file
    file.close();
}
