#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

//derived from abstract class hittable
class sphere : public hittable
{
    public:
    sphere() {} //constructor
    sphere(point3 cen, double r) : centre(cen), radius(r) {};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
    point3 centre;
    double radius;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const 
{
    vec3 co = r.origin() - centre;
    auto a = r.direction().length_squared();
    auto half_b = dot((r.direction()), co);
    auto c = co.length_squared() - radius*radius;


    auto discriminant = half_b*half_b - a*c;
    
    if(discriminant<0) return false;

    auto sqrtd = sqrt(discriminant);

    auto root = (-half_b - sqrtd) / a;
    //test to see if the root is within acceptable regions
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a; // try other root
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - centre) / radius;

    //change normal so it always faces against the ray
    //also sets front_face
    rec.set_face_normal(r, outward_normal);

    return true;
}

#endif