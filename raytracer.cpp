#include <iostream>
#include <Windows.h>
#include <cstdio>
#include "rtweekend.h"

#include "colour.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(colour(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = colour::random() * colour::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = colour::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(colour(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(colour(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}


int main(int argc, char** argv)
{
    std::cout << "Test0" << std::endl;
    //argv layout:
    // [1]:image_name [2-4]:lookat [5-7]:lookfrom
    char* image_name = argv[1];




    //Image
    
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 100;
    
    //gamma correction scaling
    auto scale = 1.0 / samples_per_pixel;


    // World
    auto world = random_scene();

    //Camera
    point3 lookat(std::atof(argv[2]), std::atof(argv[3]), std::atof(argv[4]));
    point3 lookfrom(std::atof(argv[5]), std::atof(argv[6]), std::atof(argv[7]));
    vec3 vup(0,1,0);
    auto dist_to_focus = (lookfrom-lookat).length();
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    
    //Render
    // std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    int index = 0;
    unsigned char pixel_data[image_height * image_width * 3];
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
            //gamma correction
            
            pixel_data[index++] = static_cast<int>(256 * clamp(sqrt(scale*pixel_colour.x()), 0.0, 0.999));
            pixel_data[index++] = static_cast<int>(256 * clamp(sqrt(scale*pixel_colour.y()), 0.0, 0.999));
            pixel_data[index++] = static_cast<int>(256 * clamp(sqrt(scale*pixel_colour.z()), 0.0, 0.999));
            // write_colour(std::cout, pixel_colour, samples_per_pixel);
        }
    }
    stbi_write_jpg(image_name, image_width, image_height, 3, pixel_data, 100);

    std::cerr << " \nDone.\n";
    
}
