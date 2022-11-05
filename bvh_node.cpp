#include "bvh_node.h"
#include "utils.h"
#include <iostream>

inline bool box_compare(const QSharedPointer<Hittable> &a, const QSharedPointer<Hittable> &b, int axis)
{
    AABB box1;
    AABB box2;

    if (!a->get_bounding_box(box1) || !b->get_bounding_box(box2))
    {
        std::cerr << "No bounding box when building bvh tree." << std::endl;
        exit(1);
    }

    return box1.min()[axis] < box2.min()[axis];
}

BVHNode::BVHNode(const HittableList &hittable_list)
    : BVHNode(hittable_list.objects(), 0, hittable_list.objects().size())
{
}

BVHNode::BVHNode(const QVector<QSharedPointer<Hittable>> &src_objects, size_t start, size_t end)
{
    auto objects = src_objects;
    int axis = random_int(0, 2);
    auto comparator = [=](const QSharedPointer<Hittable> &a, const QSharedPointer<Hittable> &b)
    {
        return box_compare(a, b, axis);
    };

    size_t size = end - start;
    if (size == 1)
    {
        _left = _right = objects[start];
    }
    else if (size == 2)
    {
        if (comparator(objects[start], objects[start + 1]))
        {
            _left = objects[start];
            _right = objects[start + 1];
        }
        else
        {
            _left = objects[start + 1];
            _right = objects[start];
        }
    }
    else
    {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);
        auto mid = start + size / 2;
        _left = QSharedPointer<BVHNode>::create(objects, start, mid);
        _right = QSharedPointer<BVHNode>::create(objects, mid, end);
    }

    AABB box_left, box_right;
    if (!_left->get_bounding_box(box_left) || !_right->get_bounding_box(box_right))
    {
        std::cerr << "No bounding box when building bvh tree." << std::endl;
        exit(1);
    }

    _box = get_combined_box(box_left, box_right);
}

bool BVHNode::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    if (!_box.hit(ray, t_min, t_max))
    {
        return false;
    }
    bool hit_left = _left->hit(ray, t_min, t_max, rec);
    bool hit_right = _right->hit(ray, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

bool BVHNode::get_bounding_box(AABB &aabb) const
{
    aabb = _box;
    return true;
}
