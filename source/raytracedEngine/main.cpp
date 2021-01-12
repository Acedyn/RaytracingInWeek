#include "utility.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

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
    const int samples_per_pixel = 20;

    // World assets
    hittable_list world;
    world.add(std::make_shared<sphere>(vec3(0,0,-1), 0.5));
    world.add(std::make_shared<sphere>(vec3(0,-100.5,-1), 100));

    // Create a camera that will create rays
    camera cam;

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
            // Initialize color with default value
            color pixel_color(0, 0, 0);
            // Loop over all the samples for this pixel
            for (int s = 0; s < samples_per_pixel; ++s) 
            {
                // Get the normalized coordinates of the pixel
                double u = (double(i) + random_double()) / (IMG_WIDTH-1);
                double v = (double(j) + random_double()) / (IMG_HEIGHT-1);
                // Get the ray from the camera to this pixel
                ray r = cam.get_ray(u, v);
                // Compute the ray color and add it to the pixel color
                pixel_color += ray_color(r, world);
            }
            // Average the color between all the samples and write it out
            write_color(file, pixel_color, samples_per_pixel);
        }
    }

    // Output the file
    file.close();
}
