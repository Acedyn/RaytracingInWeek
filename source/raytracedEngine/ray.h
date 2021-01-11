#pragma once

#include "vec3.h"

// Ray usually casted from a camera onto geometry
class ray {
    public:
        // Constructors / Desctructors
        ray() {}
        ray(const point3& origin, const vec3& direction)
            : orig(origin), dir(direction)
        {}

        // Getters / Setters
        point3 origin() const  { return orig; }
        vec3 direction() const { return dir; }

        // Get the hit location
        point3 at(double t) const 
        {
            return orig + t*dir;
        }

    public:
        point3 orig;
        vec3 dir;
};
