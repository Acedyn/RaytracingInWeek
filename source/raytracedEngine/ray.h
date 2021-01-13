#pragma once

#include "vec3.h"

// Ray usually casted from a camera onto geometry
class ray 
{
public:
    // Constructors / Desctructors
    ray() {}
    ray(const point3& origin, const vec3& direction, double time = 0.0f)
        : orig(origin), dir(direction), tm(time)
    {}

    // Getters / Setters
    point3 origin() const  { return orig; }
    vec3 direction() const { return dir; }
    double time() const    { return tm; }

    // Get the hit location
    point3 at(double t) const 
    {
        return orig + t*dir;
    }

public:
    point3 orig;
    vec3 dir;
    double tm; // time
};
