#ifndef CAMERA_H
#define CAMERA_H
#include "custom.h"
#include "ray.h"

class Camera
{
public:
    static Camera createFromJson(const QString &path, bool *ok = nullptr);

public:
    Camera() = default;
    Camera(const Point &eye, const Point &lookat, const QVector3D &up, double vfov);

    Ray get_ray(double u, double v) const;

private:
    Point _origin;
    QVector3D _u;
    QVector3D _v;
    QVector3D _w;
    Point _left_bottom_corner;
    QVector3D _horizontal;
    QVector3D _vertical;
};

#endif // CAMERA_H
