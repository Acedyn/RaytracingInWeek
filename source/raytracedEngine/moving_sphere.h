#pragma once

#include "utility.h"
#include "hittable.h"


class moving_sphere : public hittable 
{
public:
    moving_sphere() {}
    moving_sphere(
        point3 cen0, point3 cen1, double _time0, double _time1, double r, std::shared_ptr<material> m)
        : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m)
    {};

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

    point3 center(double time) const;

public:
    point3 center0, center1;
    double time0, time1;
    double radius;
    std::shared_ptr<material> mat_ptr;
};

point3 moving_sphere::center(double time) const 
{
    return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}

bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const 
{
    // Same method as sphere::hit but we offset the center with r.time

    // t_min = minimum ray lenght
    // t_max = maximum ray lenght

    // Equation to compute if the ray hit the sphere
    // We need to find the distance t where the position ray.direction * t
    // is at r distance of the center of the sphere (where r is the radius of that sphere)
    vec3 oc = r.origin() - center(r.time());
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
    rec.normal = (rec.p - center(r.time())) / radius;
    rec.mat_ptr = mat_ptr;

    // Store the orientation of the normal from the orientation of the ray
    // to check if we hit the face from the front or from the face
    vec3 outward_normal = (rec.p - center(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}
