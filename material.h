#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

struct hit_record;

class material
{
    public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
    public:
    lambertian(const colour& a) : albedo(a) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const override;



    public:
    colour albedo;
};
bool lambertian::scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const
{
    auto scatter_direction = rec.normal + random_unit_vector();

    //lambertian materials scatter rays in random direction
    //Catch near 0 scatters
    if (scatter_direction.near_zero())
    {
        scatter_direction = rec.normal;
    }
    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
    
}

class metal : public material {
    public:
        metal(const colour& a) : albedo(a) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const override;

    public:
        colour albedo;
};

bool metal::scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const
{
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    //metals scatter rays by reflecting them
    scattered = ray(rec.p, reflected);
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
}


#endif