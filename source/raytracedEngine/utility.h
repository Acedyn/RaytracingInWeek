#pragma once

#include <cmath>
#include <limits>
#include <memory>


// Constants

const double INFINITE = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) 
{
    return degrees * PI / 180.0;
}

// Common Headers

#include "ray.h"
#include "vec3.h"
