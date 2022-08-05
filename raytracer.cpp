#include <iostream>
#include "vec3.h"
#include "colour.h"
#include "ray.h"

//return true if the ray intersects a defined sphere
bool hit_sphere(const point3& centre, double radius, const ray& r)
{
    vec3 co = r.origin() - centre;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0*dot((r.direction()), co);
    auto c = dot(co,co) - radius*radius;

    auto discriminant = b*b - 4*a*c;
    
    return (discriminant > 0);

}

colour ray_colour(const ray& r)
{
    if(hit_sphere(point3(0,0,-1) , 0.5, r))
    {
        
        return colour(1,0,0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*colour(1.0,1.0,1.0) + t*colour(0.5,0.7,1.0);
}




int main()
{
    //Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    //Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0,0,0);
    auto horizontal = vec3(viewport_width,0,0);
    auto vertical = vec3(0,viewport_height,0);
    auto focal_vect = vec3(0,0,focal_length);

    auto lower_left_corner = origin - horizontal/2 - vertical/2 - focal_vect;

    //Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j>=0; --j)
    {
        std::cerr << "\r Scanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            colour pixel_colour = ray_colour(r);
            write_colour(std::cout, pixel_colour);
        }
    }

    std::cerr << " \nDone.\n";
}
//test test test