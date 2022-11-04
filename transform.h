#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "hittable.h"
#include "aabb.h"

class Transform : public Hittable
{
public:
    Transform(QSharedPointer<Hittable> src, const QMatrix4x4 &matrix);

    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool get_bounding_box(AABB &aabb) const override;

private:
    QSharedPointer<Hittable> _src;
    QMatrix4x4 _matrix;
    QMatrix4x4 _inverted_matrix;
    bool _has_box;
    AABB _box;
};

#endif // TRANSFORM_H
