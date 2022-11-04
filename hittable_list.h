#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
#include "hittable.h"
#include <QVector>

class HittableList : public Hittable
{
public:
    HittableList() = default;

    void clear() { _objects.clear(); }
    void add(QSharedPointer<Hittable> object) { _objects.push_back(object); }
    QVector<QSharedPointer<Hittable>> objects() const noexcept { return _objects; }

    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool get_bounding_box(AABB &aabb) const override;

private:
    QVector<QSharedPointer<Hittable>> _objects;
};

#endif // HITTABLE_LIST_H
