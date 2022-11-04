#ifndef SPHERE_H
#define SPHERE_H
#include "hittable.h"

class Sphere : public Hittable
{
public:
    Sphere() = default;
    Sphere(const Point &center, double radius, QSharedPointer<Material> material_ptr)
        : _center(center), _radius(radius), _material_ptr(material_ptr) {}
    Point center() const noexcept { return _center; }
    double radius() const noexcept { return _radius; }

    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool get_bounding_box(AABB &aabb) const override;

private:
    static void get_uv(const Point &p, double &u, double &v);

private:
    Point _center;
    double _radius;
    QSharedPointer<Material> _material_ptr;
};

#endif // SPHERE_H
