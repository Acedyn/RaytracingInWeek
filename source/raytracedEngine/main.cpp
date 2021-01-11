#include "color.h"
#include "ray.h"

#include <iostream>
#include <fstream>


// Return the background (like a default color)
color ray_color(const ray& r)
{
    // Get the normalized vector
    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
}

int main()
{
    // Image size
    const double ASPECT_RATIO = 16.0f / 9.0f;
    const int IMG_WIDTH = 400;
    const int IMG_HEIGHT = static_cast<int>(IMG_WIDTH / ASPECT_RATIO);

    // Camera properties
    double viewport_height = 2.0f;
    double viewport_width = ASPECT_RATIO * viewport_height;
    double focal_length = 1.0;

    // Camera transform
    vec3 origin = vec3(0.0f, 0.0f, 0.0f);
    vec3 horizontal = vec3(viewport_width, 0.0f, 0.0f);
    vec3 vertical = vec3(0.0f, viewport_height, 0.0f);
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
            // Compute the color of the current pixel
            color pixel_color(double(i) / (IMG_WIDTH - 1), double(j) / (IMG_HEIGHT - 1), 0.25f);
            // Store the color
            write_color(file, pixel_color);
        }
    }

    // Output the file
    file.close();
}
