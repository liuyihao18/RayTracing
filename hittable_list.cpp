#include "hittable_list.h"
#include "aabb.h"

bool HittableList::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest = t_max;

    for (const auto &object : _objects)
    {
        if (object->hit(ray, t_min, closest, temp_rec))
        {
            hit_anything = true;
            closest = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool HittableList::get_bounding_box(AABB &aabb) const
{
    if (_objects.empty())
    {
        return false;
    }

    AABB temp;
    bool first_box = true;

    for (const auto &object : _objects)
    {
        if (!object->get_bounding_box(temp))
            return false;
        aabb = first_box ? temp : get_combined_box(aabb, temp);
        first_box = false;
    }

    return true;
}
