#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "hittable.h"
#include "aabb.h"

class Triangle : public Hittable
{
public:
    Triangle() = default;
    Triangle(const Point& p1, const Point& p2, const Point& p3, QSharedPointer<Material> material_ptr);

    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool get_bounding_box(AABB &aabb) const override;

private:
    Point _p1, _p2, _p3;
    double _n_dot_p1;
    QVector3D _n;
    QVector3D _p1p2, _p1p3;
    QSharedPointer<Material> _material_ptr;
    AABB _box;
};

#endif // TRIANGLE_H
