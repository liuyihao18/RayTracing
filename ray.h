#ifndef RAY_H
#define RAY_H
#include "custom.h"

class Ray
{
public:
    Ray() = default;
    Ray(const Point &origin, const QVector3D &direction) : _origin(origin), _direction(direction) {}

    Point origin() const noexcept { return _origin; }
    QVector3D direction() const noexcept { return _direction; }

    Point at(double t) const noexcept
    {
        return _origin + t * _direction;
    }

private:
    Point _origin;
    QVector3D _direction;
};

#endif // RAY_H
