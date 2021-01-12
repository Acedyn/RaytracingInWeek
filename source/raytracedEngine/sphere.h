#pragma once

#include "hittable.h"
#include "utility.h"

class sphere : public hittable 
{
    public:
        sphere() {}
        sphere(vec3 center, double radius, std::shared_ptr<material> m) : 
            center(center), 
            radius(radius),
            mat_ptr(m) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center;
        double radius;
        std::shared_ptr<material> mat_ptr;
};

// Test of a ray is hitting this sphere
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    // t_min = minimum ray lenght
    // t_max = maximum ray lenght

    // Equation to compute if the ray hit the sphere
    // We need to find the distance t where the position ray.direction * t
    // is at r distance of the center of the sphere (where r is the radius of that sphere)
    vec3 oc = r.origin() - center;
    double a = r.direction().length_squared();
    double half_b = dot(oc, r.direction());
    double c = oc.length_squared() - radius*radius;

    double discriminant = half_b*half_b - a*c;
    // If the discriminant is 0 -> no hit
    // If the discriminant is 1 -> 1 hit
    // If the discriminant is 2 -> more than one hit
    if (discriminant < 0) return false;
    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) 
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;
    rec.mat_ptr = mat_ptr;

    // Store the orientation of the normal from the orientation of the ray
    // to check if we hit the face from the front or from the face
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}
