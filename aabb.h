#ifndef AABB_H
#define AABB_H
#include "custom.h"
#include "ray.h"

class AABB
{
public:
    AABB() = default;
    AABB(const Point &min, const Point &max) : _min(min), _max(max) {}

    Point min() const noexcept { return _min; }
    Point max() const noexcept { return _max; }

    bool hit(const Ray &ray, double t_min, double t_max) const
    {
        for (int i = 0; i < 3; i++)
        {
            auto invD = 1.0 / ray.direction()[i];
            auto t0 = (_min[i] - ray.origin()[i]) * invD;
            auto t1 = (_max[i] - ray.origin()[i]) * invD;
            if (invD < 0.0)
                qSwap(t0, t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min)
            {
                return false;
            }
        }
        return true;
    }

private:
    Point _min; // 靠近坐标原点的点
    Point _max; // 远离坐标原点的点
};

inline AABB get_combined_box(const AABB &box1, const AABB &box2)
{
    Point a(qMin(box1.min().x(), box2.min().x()),
            qMin(box1.min().y(), box2.min().y()),
            qMin(box1.min().z(), box2.min().z()));
    Point b(qMax(box1.max().x(), box2.max().x()),
            qMax(box1.max().y(), box2.max().y()),
            qMax(box1.max().z(), box2.max().z()));
    return AABB(a, b);
}

#endif // AABB_H
