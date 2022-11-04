#ifndef MESH_H
#define MESH_H
#include "hittable.h"
#include "hittable_list.h"
#include "bvh_node.h"

class Mesh : public Hittable
{
public:
    Mesh() = default;
    Mesh(const QString &filename, QSharedPointer<Material> material_ptr);

    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool get_bounding_box(AABB &aabb) const override;

private:
    HittableList _triangles;
    BVHNode _root;
};

#endif // MESH_H
