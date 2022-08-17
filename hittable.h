#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "rtweekend.h"
class material;

struct hit_record
{
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        //check if ray and outward normal are facing the same direction
        //if they are we are inside the sphere
        front_face = dot(r.direction(), outward_normal) < 0;

        //flip the normal if so. (normal always points against the ray)
        normal = front_face ? outward_normal :-outward_normal;
    }
};

//Abstract class
class hittable
{
    public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0; //virtual pure function
};

#endif
