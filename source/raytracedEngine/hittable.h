#pragma once

#include "ray.h"
#include "utility.h"

// Output of a hit test
struct hit_record 
{
    // Hit position
    point3 p;
    // Hit normal
    vec3 normal;
    // Hit distance
    double t;
    
    // Hit orientation of the normal
    bool front_face;
    inline void set_face_normal(const ray& r, const vec3& outward_normal) 
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};

// Interface for object that wan be rendered
class hittable 
{
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
