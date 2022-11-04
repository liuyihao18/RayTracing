#include "transform.h"
#include "ray.h"

Transform::Transform(QSharedPointer<Hittable> src,
                     const QMatrix4x4 &matrix)
    : _src(src), _matrix(matrix)
{
    _inverted_matrix = _matrix.inverted();

    _has_box = _src->get_bounding_box(_box);

    Point min(INFINITY, INFINITY, INFINITY);
    Point max(-INFINITY, -INFINITY, -INFINITY);

    for (int i = 0; i < 8; i++)
    {
        auto x = (i & 1) * _box.min().x() + (1 - (i & 1)) * _box.max().x();
        auto y = ((i >> 1) & 1) * _box.min().y() + (1 - ((i >> 1) & 1)) * _box.max().y();
        auto z = ((i >> 2) & 1) * _box.min().z() + (1 - ((i >> 2) & 1)) * _box.max().z();

        QVector4D p(x, y, z, 1.0);
        p = _matrix * p;

        for (int a = 0; a < 3; a++)
        {
            min[a] = qMin(min[a], p[a]);
            max[a] = qMax(max[a], p[a]);
        }
    }

    _box = AABB(min, max);
}

bool Transform::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    auto origin = QVector4D(ray.origin(), 1.0);
    auto direction = QVector4D(ray.direction(), 0.0);

    origin = _inverted_matrix * origin;
    direction = _inverted_matrix * direction;

    Ray transfromed_ray(origin.toVector3D(), direction.toVector3D());

    if (!_src->hit(transfromed_ray, t_min, t_max, rec))
    {
        return false;
    }

    auto p = QVector4D(rec.p, 1.0);
    auto n = QVector4D(rec.n, 0.0);

    p = _matrix * p;
    n = _matrix * n;

    rec.p = p.toVector3D();
    rec.set_face_normal(ray, n.toVector3D());

    return true;
}

bool Transform::get_bounding_box(AABB &aabb) const
{
    aabb = _box;
    return _has_box;
}
