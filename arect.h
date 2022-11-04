#ifndef ARECT_H
#define ARECT_H
#include "hittable.h"

class XYRect : public Hittable
{
public:
    XYRect() = default;
    XYRect(double x0, double x1, double y0, double y1, double k, QSharedPointer<Material> material_ptr) : _x0(x0), _x1(x1), _y0(y0), _y1(y1), _k(k), _material_ptr(material_ptr) {}

    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool get_bounding_box(AABB &aabb) const override;

private:
    double _x0, _x1, _y0, _y1, _k;
    QSharedPointer<Material> _material_ptr;
};

class XZRect : public Hittable
{
public:
    XZRect() = default;
    XZRect(double x0, double x1, double z0, double z1, double k, QSharedPointer<Material> material_ptr) : _x0(x0), _x1(x1), _z0(z0), _z1(z1), _k(k), _material_ptr(material_ptr) {}

    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool get_bounding_box(AABB &aabb) const override;

private:
    double _x0, _x1, _z0, _z1, _k;
    QSharedPointer<Material> _material_ptr;
};

class YZRect : public Hittable
{
public:
    YZRect() = default;
    YZRect(double y0, double y1, double z0, double z1, double k, QSharedPointer<Material> material_ptr) : _y0(y0), _y1(y1), _z0(z0), _z1(z1), _k(k), _material_ptr(material_ptr) {}

    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool get_bounding_box(AABB &aabb) const override;

private:
    double _y0, _y1, _z0, _z1, _k;
    QSharedPointer<Material> _material_ptr;
};

#endif // ARECT_H
