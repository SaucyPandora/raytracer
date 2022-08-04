#include <iostream>
#include "vec3.h"
#include "colour.h"

int main()
{
    //Image
    const int image_width = 256;
    const int image_height = 256;

    //Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j>=0; --j)
    {
        std::cerr << "\r Scanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            colour pixel_colour(double(i)/(image_width-1), double(j)/(image_height-1), 0.25);
            write_colour(std::cout, pixel_colour);
        }
    }

    std::cerr << " \nDone.\n";
}
//test test test