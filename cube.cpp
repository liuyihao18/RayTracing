#include "cube.h"
#include "arect.h"
#include "aabb.h"

Cube::Cube(const Point &min, const Point &max, QSharedPointer<Material> material_ptr)
    : _min(min), _max(max)
{
    _faces.add(QSharedPointer<XYRect>::create(min.x(), max.x(), min.y(), max.y(), min.z(), material_ptr)); // 前面
    _faces.add(QSharedPointer<XYRect>::create(min.x(), max.x(), min.y(), max.y(), max.z(), material_ptr)); // 后面
    _faces.add(QSharedPointer<YZRect>::create(min.y(), max.y(), min.z(), max.z(), min.x(), material_ptr)); // 左面
    _faces.add(QSharedPointer<YZRect>::create(min.y(), max.y(), min.z(), max.z(), max.x(), material_ptr)); // 右面
    _faces.add(QSharedPointer<XZRect>::create(min.x(), max.x(), min.z(), max.z(), min.y(), material_ptr)); // 下面
    _faces.add(QSharedPointer<XZRect>::create(min.x(), max.x(), min.z(), max.z(), max.y(), material_ptr)); // 上面
}

bool Cube::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    return _faces.hit(ray, t_min, t_max, rec);
}

bool Cube::get_bounding_box(AABB &aabb) const
{
    aabb = AABB(_min, _max);
    return true;
}
