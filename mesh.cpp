#include "mesh.h"
#include "triangle.h"
#include "OBJ_Loader.h"
#include <iostream>

Mesh::Mesh(const QString &filename, QSharedPointer<Material> material_ptr)
{
    objl::Loader loader;
    if (!loader.LoadFile(filename.toStdString()))
    {
        std::cerr << "Error when loading obj file." << std::endl;
    }

    auto mesh = loader.LoadedMeshes[0];
    for (int i = 0; i < mesh.Vertices.size(); i += 3)
    {
        auto p1 = Point(mesh.Vertices[i + 0].Position.X,
                        mesh.Vertices[i + 0].Position.Y,
                        mesh.Vertices[i + 0].Position.Z);
        auto p2 = Point(mesh.Vertices[i + 1].Position.X,
                        mesh.Vertices[i + 1].Position.Y,
                        mesh.Vertices[i + 1].Position.Z);
        auto p3 = Point(mesh.Vertices[i + 2].Position.X,
                        mesh.Vertices[i + 2].Position.Y,
                        mesh.Vertices[i + 2].Position.Z);
        _triangles.add(QSharedPointer<Triangle>::create(p1, p2, p3, material_ptr));
    }

    _root = BVHNode(_triangles);
}

bool Mesh::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    return _root.hit(ray, t_min, t_max, rec);
}

bool Mesh::get_bounding_box(AABB &aabb) const
{
    return _root.get_bounding_box(aabb);
}
