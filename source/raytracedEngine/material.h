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
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(
        const ray& r_in, 
        const hit_record& rec, 
        color& attenuation, 
        ray& scattered) const override 
    {
        // Get the reflected vector from the normal and the input ray
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        // Create the output ray for the reflected vector
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        // Create the output color thos the hit record
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    color albedo;
    double fuzz;
};

class dielectric : public material 
{
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(
        const ray& r_in, 
        const hit_record& rec, 
        color& attenuation, 
        ray& scattered) const override
    {
        // There is no color so we return 1 since it absorbs nothing
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        // Test is the refraction is possible or not
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        // If the refraction is not possible return the reflected vector
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, rec.normal);
        // If the refraction is possible return the refracted vector
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);


        // Create the output ray from the refracted vector
        scattered = ray(rec.p, direction);
        return true;
    }

public:
    double ir; // Index of Refraction

private:
    static double reflectance(double cosine, double ref_idx) 
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};
