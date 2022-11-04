#ifndef BVH_NODE_H
#define BVH_NODE_H
#include "hittable.h"
#include "hittable_list.h"
#include "aabb.h"

class BVHNode : public Hittable
{
public:
    BVHNode() = default;
    BVHNode(const HittableList &hittable_list);
    BVHNode(const QVector<QSharedPointer<Hittable>> &src_objects, size_t start, size_t end);

    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool get_bounding_box(AABB &aabb) const override;

private:
    QSharedPointer<Hittable> _left;
    QSharedPointer<Hittable> _right;
    AABB _box;
};

#endif // BVH_NODE_H
