#include "utility.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>
#include <fstream>

// Get the hit color, if no hit, return the background (like a default color)
color ray_color(const ray& r, const hittable& world, int depth)
{
    hit_record rec;
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) { return color(0,0,0); }
    // Test if the ray hit one of the objects
    if (world.hit(r, 0.001, INFINITE, rec)) 
    {
        // Ray that will be overriden by scatter() (passed by reference)
        ray scattered;
        // Colot that will be overriden by scatter() (passed by reference)
        color attenuation;
        // Get the info of the material and pass them to attenuation and scattered
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            // Go for an other iteration with the new bounced ray
            return attenuation * ray_color(scattered, world, depth-1);
        }
        return color(0,0,0);
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
    const int samples_per_pixel = 10;
    const int max_depth = 10;

    // World assets
    hittable_list world;

    auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<dielectric>(1.5);
    auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(std::make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0), -0.45, material_left));
    world.add(std::make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Create a camera that will create rayspoint3 lookfrom(3,3,2);
    point3 lookat(0,0,-1);
    point3 lookfrom(3,3,2);
    vec3 vup(0,1,0);
    auto dist_to_focus = (lookfrom-lookat).length();
    auto aperture = 2.0;

    camera cam(lookfrom, lookat, vup, 20, ASPECT_RATIO, aperture, dist_to_focus);

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
                pixel_color += ray_color(r, world, max_depth);
            }
            // Average the color between all the samples and write it out
            write_color(file, pixel_color, samples_per_pixel);
        }
    }

    // Output the file
    file.close();
}
