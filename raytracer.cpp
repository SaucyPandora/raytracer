#include <iostream>
#include "vec3.h"
#include "colour.h"
#include "ray.h"

//return true if the ray intersects a defined sphere
double hit_sphere(const point3& centre, double radius, const ray& r)
{
    vec3 co = r.origin() - centre;
    auto a = r.direction().length_squared();
    auto half_b = dot((r.direction()), co);
    auto c = co.length_squared() - radius*radius;


    auto discriminant = half_b*half_b - a*c;
    
    if(discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (-half_b - sqrt(discriminant)) / (a);
    }

}

colour ray_colour(const ray& r)
{
    auto t = hit_sphere(point3(0,0,-1), 0.5, r); //returns the t value for where the ray intersects the sphere
    if (t > 0.0)
    {
        vec3 N = unit_vector(r.at(t)-vec3(0,0,-1)); // N is the vector from the centre of the sphere to the point of intersection, normalised.
        return 0.5*colour(N.x()+1, N.y()+1, N.z()+1); // Map N to a rgb value 0-1
    }

    //Blue gradient background
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5*(unit_direction.y() + 1.0);
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