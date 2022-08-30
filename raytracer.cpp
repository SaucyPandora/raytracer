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
    hittable_list world;

    auto material_ground = make_shared<lambertian>(colour(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(colour(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(colour(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0), -0.45, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    //Camera
    point3 lookfrom(3,3,2);
    point3 lookat(0,0,-1);
    vec3 vup(0,1,0);
    auto dist_to_focus = (lookfrom-lookat).length();
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    
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