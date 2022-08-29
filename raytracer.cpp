#include <iostream>
#include "rtweekend.h"

#include "colour.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

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

colour ray_colour(const ray& r, const hittable& world, int depth)
{
    //Set a ray bounce limit to cap the amount of light gathered.
    if(depth <=0)
        return colour(0,0,0);
    //Check if the ray hits any of the objects in the hittable_list
    hit_record rec;
    if (world.hit(r,0.001,infinity,rec))
    {
        ray scattered;
        colour attenuation;
        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_colour(scattered, world, depth-1);
        return colour(0,0,0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1);
    return (1.0-t)*colour(1.0, 1.0, 1.0) + t*colour(0.5, 0.7, 1.0);
    
}




int main()
{
    //Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World
    auto R = cos(pi/4);
    hittable_list world;

    auto material_left  = make_shared<lambertian>(colour(0,0,1));
    auto material_right = make_shared<lambertian>(colour(1,0,0));

    world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    world.add(make_shared<sphere>(point3( R, 0, -1), R, material_right));
    //Camera
    camera cam(90.0, aspect_ratio);

    //Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j>=0; --j)
    {
        std::cerr << "\r Scanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            colour pixel_colour(0,0,0);
            for(int s = 0; s< samples_per_pixel; ++s)
            {
                auto u = double(i + random_double()) / (image_width-1);
                auto v = double(j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u,v);
                pixel_colour += ray_colour(r, world, max_depth);
            }

            write_colour(std::cout, pixel_colour, samples_per_pixel);
        }
    }

    std::cerr << " \nDone.\n";
    
}
//test test test