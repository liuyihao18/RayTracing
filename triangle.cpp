#include "triangle.h"
#include "double.h"
#include "gui_handler.h"

Triangle::Triangle(const Point &p1, const Point &p2, const Point &p3, QSharedPointer<Material> material_ptr)
    : _p1(p1), _p2(p2), _p3(p3), _material_ptr(material_ptr)
{
    _p1p2 = p2 - p1;
    _p1p3 = p3 - p1;
    if (eq(_p1p2.length(), 0) || eq(_p1p3.length(), 0))
    {
        GUIHandler::Inst()->err("网格点重合");
    }

    if (eq(qAbs(QVector3D::dotProduct(_p1p2, _p1p3)), _p1p2.length() * _p1p3.length()))
    {
        GUIHandler::Inst()->err("网格线平行");
        exit(1);
    }

    _n = QVector3D::crossProduct(_p1p2, _p1p3).normalized();
    _n_dot_p1 = QVector3D::dotProduct(_n, _p1);

    double xxx[3] = {_p1.x(), _p2.x(), _p3.x()};
    double min_x = *std::min_element(std::begin(xxx), std::end(xxx));
    double max_x = *std::max_element(std::begin(xxx), std::end(xxx));
    if (eq(min_x, max_x))
        max_x += 0.0001;

    double yyy[3] = {_p1.y(), _p2.y(), _p3.y()};
    double min_y = *std::min_element(std::begin(yyy), std::end(yyy));
    double max_y = *std::max_element(std::begin(yyy), std::end(yyy));
    if (eq(min_y, max_y))
        max_y += 0.0001;

    double zzz[3] = {_p1.z(), _p2.z(), _p3.z()};
    double min_z = *std::min_element(std::begin(zzz), std::end(zzz));
    double max_z = *std::max_element(std::begin(zzz), std::end(zzz));
    if (eq(min_z, max_z))
        max_z += 0.0001;

    _box = AABB(Point(min_x, min_y, min_z), Point(max_x, max_y, max_z));
}

bool Triangle::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    auto t = (_n_dot_p1 - QVector3D::dotProduct(_n, ray.origin())) / (QVector3D::dotProduct(_n, ray.direction()));
    if (lt(t, t_min) || gt(t, t_max))
    {
        return false;
    }

    auto p = ray.at(t);
    auto pp1 = _p1 - p;
    auto pp2 = _p2 - p;
    auto pp3 = _p3 - p;
    if (ne(pp1.length(), 0) && ne(pp2.length(), 0) && ne(pp3.length(), 0))
    {
        auto pp1_x_pp2 = QVector3D::crossProduct(pp1, pp2);
        auto pp2_x_pp3 = QVector3D::crossProduct(pp2, pp3);
        auto pp3_x_pp1 = QVector3D::crossProduct(pp3, pp1);
        bool flag = QVector3D::dotProduct(pp1_x_pp2, pp2_x_pp3) > 0;
        if (!flag || QVector3D::dotProduct(pp2_x_pp3, pp3_x_pp1) < 0)
        {
            return false;
        }
    }

    rec.t = t;
    rec.p = p;
    rec.set_face_normal(ray, _n);
    /* not support texture */
    rec.u = 0;
    rec.v = 0;
    rec.material_ptr = _material_ptr;

    return true;
}

bool Triangle::get_bounding_box(AABB &aabb) const
{
    aabb = _box;
    return true;
}
