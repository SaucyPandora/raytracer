#include <iostream>



int main()
{
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    const int image_height2 = (int) image_width / aspect_ratio;

    std::cout << image_height << ',' << image_height2;
}