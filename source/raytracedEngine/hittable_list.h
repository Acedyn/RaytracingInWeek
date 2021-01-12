#pragma once

#include "hittable.h"

#include <memory>
#include <vector>


// Class to store all the objects to test when we cast a ray
class hittable_list : public hittable 
{
    public:
        // Constructors / Destructors
        hittable_list() {}
        hittable_list(std::shared_ptr<hittable> object) { add(object); }

        // Append / Clear
        void clear() { objects.clear(); }
        void add(std::shared_ptr<hittable> object) { objects.push_back(object); }

        // Test hit over all the objects stored
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        // The actual list of hittable objects
        std::vector<std::shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const 
{
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    // Loop over all the objects
    for (const auto& object : objects) {
        // Call the hit function of the object
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
