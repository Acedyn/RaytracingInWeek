#pragma once

#include "ray.h"

// Output of a hit test
struct hit_record 
{
    point3 p;
    vec3 normal;
    double t;
};

// Interface for object that wan be rendered
class hittable 
{
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
