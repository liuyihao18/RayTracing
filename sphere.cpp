#include "sphere.h"
#include "ray.h"
#include "aabb.h"
#include "double.h"

bool Sphere::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    QVector3D oc = ray.origin() - _center;
    auto a = ray.direction().lengthSquared();
    auto half_b = QVector3D::dotProduct(ray.direction(), oc);
    auto c = oc.lengthSquared() - _radius * _radius;
    auto delta = half_b * half_b - a * c;
    if (delta < 0)
        return false;

    auto sqrt_delta = sqrt(delta);
    auto t = (-half_b - sqrt_delta) / a;
    if (lt(t, t_min) || gt(t, t_max))
    {
        t = (-half_b + sqrt_delta) / a;
    }
    if (lt(t, t_min) || gt(t, t_max))
    {
        return false;
    }

    rec.t = t;
    rec.p = ray.at(t);
    QVector3D outward_normal = (rec.p - _center) / _radius;
    rec.set_face_normal(ray, outward_normal);
    get_uv(outward_normal, rec.u, rec.v);
    rec.material_ptr = _material_ptr;

    return true;
}

bool Sphere::get_bounding_box(AABB &aabb) const
{
    aabb = AABB(
        _center - QVector3D(_radius, _radius, _radius),
        _center + QVector3D(_radius, _radius, _radius));
    return true;
}

void Sphere::get_uv(const Point &p, double &u, double &v)
{
    auto theta = qAcos(-p.y());
    auto phi = qAtan2(-p.z(), p.x()) + PI;

    u = phi / (2 * PI);
    v = theta / PI;
}
