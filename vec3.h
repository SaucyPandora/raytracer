#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 
{
    public:
        //Empty constructor
        vec3() : e{0,0,0} // member init list
        {}

        //Specified constructor
        vec3(double e0, double e1, double e2) : e{e0,e1,e2}
        {}

        double x() const {return e[0];}
        double y() const {return e[1];}
        double z() const {return e[2];}

        vec3 operator-() const { return vec3(-e[0],-e[1],-e[2]); }
        //happens when object is a constant
        double operator[](int i) const { return e[i]; }

        //happens when object is not const and returns a reference
        double& operator[](int i) { return e[i]; }

        //*this and & combination means we return a reference to the calling object
        vec3& operator+=(const vec3 &v)
        {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];

            return *this;
        }

        vec3& operator*=(const double t)
        {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;

            return *this;
        }

        vec3& operator/=(const double t)
        {
            return *this *= 1/t;
        }

        double length() const
        {
            return sqrt(length_squared());
        }

        double length_squared() const 
        {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }


    
    public:
        double e[3];
};

using point3 = vec3;
using colour = vec3;


//chapter3.2

//since this operator changes the ostream it must be declared as a member function of the left operand (hence inline because we're editing ostream)
inline std::ostream& operator<<(std::ostream &out, const vec3 &v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

//Operator treats both operands equally hence non-member functions
inline vec3 operator+(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v)
{
    return t * v;
}

inline vec3 operator/(vec3 v, double t)
{
    return (1/t)*v;
}

inline double dot(const vec3 &u, const vec3 &v)
{
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}
#endif