#include "camera.h"

Camera::Camera(const Point &eye, const Point &lookat, const QVector3D &up, double vfov)
{
    auto theta = qDegreesToRadians(vfov);
    auto h = qTan(theta / 2.0);
    auto viewport_height = 2.0 * h;
    auto viewport_width = viewport_height * aspect_ratio;

    _w = (eye - lookat).normalized();     // w轴
    _u = QVector3D::crossProduct(up, _w); // u轴
    _v = QVector3D::crossProduct(_w, _u); // v轴

    _origin = eye;
    _horizontal = viewport_width * _u; // 投影平面水平方向（u轴）
    _vertical = viewport_height * _v;  // 投影平面竖直方向（v轴）
    _left_bottom_corner = _origin - _horizontal / 2 - _vertical / 2 - _w;
}

Ray Camera::get_ray(double s, double t) const
{
    return Ray(
        _origin,
        _left_bottom_corner + s * _horizontal + t * _vertical - _origin
    );
}
