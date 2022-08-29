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
        metal(const colour& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const override;

    public:
        colour albedo;
        double fuzz;
};

bool metal::scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const
{
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    //metals scatter rays by reflecting them
    scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
}

class dielectric : public material {
    public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const override;

    public:
    double ir;

    private:
    static double reflectance(double cosine, double ref_idx)
    {
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }




};

bool dielectric::scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const {
    attenuation = colour(1.0,1.0,1.0);
    double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

    vec3 unit_direction = unit_vector(r_in.direction());

    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1 - cos_theta*cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;

    vec3 direction;
    //If the angle of incidence is above the critical angle then we reflect instead of refracting
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
        direction = reflect(unit_direction, rec.normal);
    else
        direction = refract(unit_direction, rec.normal, refraction_ratio);

    //scattered ray is set to the appropriate direction
    scattered = ray(rec.p, direction);
    return true;
}

#endif