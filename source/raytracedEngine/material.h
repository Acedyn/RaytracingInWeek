#pragma once

#include "utility.h"
#include "hittable.h"

struct hit_record;

class material 
{
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material 
{
public:
    lambertian(const color& a) : albedo(a) {}

    virtual bool scatter(
        const ray& r_in, 
        const hit_record& rec, 
        color& attenuation, 
        ray& scattered) const override 
    {
        // Shif the normal randomly for diffuse look
        vec3 scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        // Create a bouning ray from the hit record (returned by reference)
        scattered = ray(rec.p, scatter_direction);
        // Create a color from the hit record (returned by reference)
        attenuation = albedo;
        return true;
    }

public:
    color albedo;
};

class metal : public material 
{
public:
    metal(const color& a) : albedo(a) {}

    virtual bool scatter(
        const ray& r_in, 
        const hit_record& rec, 
        color& attenuation, 
        ray& scattered) const override 
    {
        // Get the reflected vector from the normal and the input ray
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        // Create the output ray for the reflected vector
        scattered = ray(rec.p, reflected);
        // Create the output color thos the hit record
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    color albedo;
};
