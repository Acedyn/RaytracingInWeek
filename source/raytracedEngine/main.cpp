#include "color.h"

#include <iostream>
#include <fstream>


int main()
{
    // Image size
    const int IMG_WIDTH = 200;
    const int IMG_HEIGHT = 100;

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
            // Compute the color of the current pixel
            color pixel_color(double(i) / (IMG_WIDTH - 1), double(j) / (IMG_HEIGHT - 1), 0.25f);
            // Store the color
            write_color(file, pixel_color);
        }
    }

    // Output the file
    file.close();
}
