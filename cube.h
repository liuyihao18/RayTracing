#ifndef CUBE_H
#define CUBE_H
#include "hittable.h"
#include "hittable_list.h"

class Cube : public Hittable
{
public:
    Cube() = default;
    Cube(const Point &min, const Point &max, QSharedPointer<Material> material_ptr);

    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool get_bounding_box(AABB &aabb) const override;

private:
    Point _min;
    Point _max;
    HittableList _faces;
};

#endif // CUBE_H
