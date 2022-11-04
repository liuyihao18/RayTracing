#include "arect.h"
#include "ray.h"
#include "aabb.h"
#include "double.h"

bool XYRect::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    auto t = (_k - ray.origin().z()) / ray.direction().z();
    if (lt(t, t_min) || gt(t, t_max))
    {
        return false;
    }

    auto x = ray.origin().x() + t * ray.direction().x();
    auto y = ray.origin().y() + t * ray.direction().y();
    if (lt(x, _x0) || gt(x, _x1) || lt(y, _y0) || gt(y, _y1))
    {
        return false;
    }

    rec.t = t;
    rec.p = ray.at(t);
    auto outward_normal = QVector3D(0, 0, 1);
    rec.set_face_normal(ray, outward_normal);
    rec.u = (x - _x0) / (_x1 - _x0);
    rec.v = (y - _y0) / (_y1 - _y0);
    rec.material_ptr = _material_ptr;

    return true;
}

bool XYRect::get_bounding_box(AABB &aabb) const
{
    aabb = AABB(Point(_x0, _y0, _k - 0.0001), Point(_x1, _y1, _k + 0.0001));
    return true;
}

bool XZRect::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    auto t = (_k - ray.origin().y()) / ray.direction().y();
    if (lt(t, t_min) || gt(t, t_max))
    {
        return false;
    }

    auto x = ray.origin().x() + t * ray.direction().x();
    auto z = ray.origin().z() + t * ray.direction().z();
    if (lt(x, _x0) || gt(x, _x1) || lt(z, _z0) || gt(z, _z1))
    {
        return false;
    }

    rec.t = t;
    rec.p = ray.at(t);
    auto outward_normal = QVector3D(0, 1, 0);
    rec.set_face_normal(ray, outward_normal);
    rec.u = (x - _x0) / (_x1 - _x0);
    rec.v = (z - _z0) / (_z1 - _z0);
    rec.material_ptr = _material_ptr;

    return true;
}

bool XZRect::get_bounding_box(AABB &aabb) const
{
    aabb = AABB(Point(_x0, _k - 0.0001, _z0), Point(_x1, _k + 0.0001, _z1));
    return true;
}

bool YZRect::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    auto t = (_k - ray.origin().x()) / ray.direction().x();
    if (lt(t, t_min) || gt(t, t_max))
    {
        return false;
    }

    auto y = ray.origin().y() + t * ray.direction().y();
    auto z = ray.origin().z() + t * ray.direction().z();
    if (lt(y, _y0) || gt(y, _y1) || lt(z, _z0) || gt(z, _z1))
    {
        return false;
    }

    rec.t = t;
    rec.p = ray.at(t);
    auto outward_normal = QVector3D(1, 0, 0);
    rec.set_face_normal(ray, outward_normal);
    rec.u = (y - _y0) / (_y1 - _y0);
    rec.v = (z - _z0) / (_z1 - _z0);
    rec.material_ptr = _material_ptr;

    return true;
}

bool YZRect::get_bounding_box(AABB &aabb) const
{
    aabb = AABB(Point(_k - 0.0001, _y0, _z0), Point(_k + 0.0001, _y1, _z1));
    return true;
}
