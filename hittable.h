#ifndef HITTABLE_H
#define HITTABLE_H
#include "custom.h"
#include "ray.h"

struct HitRecord
{
    Point p;                               // 击中点
    QVector3D n;                           // 击中点面法向
    QSharedPointer<Material> material_ptr; // 材质
    double t;                              // 光线位置
    double u;                              // 纹理坐标
    double v;                              // 纹理坐标
    bool front_face;                       // 光线方向与面法向的关系

    void set_face_normal(const Ray &ray, const QVector3D &outward_normal);
};

inline void HitRecord::set_face_normal(const Ray &ray, const QVector3D &outward_normal)
{
    front_face = QVector3D::dotProduct(ray.direction(), outward_normal) < 0;
    n = front_face ? outward_normal : -outward_normal;
}

class Hittable
{
public:
    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const = 0;
    virtual bool get_bounding_box(AABB &aabb) const = 0;
};

#endif // HITTABLE_H
