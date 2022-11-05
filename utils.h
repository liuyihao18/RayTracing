#ifndef UTILS_H
#define UTILS_H
#include "custom.h"
#include <QColor>
#include <QJsonArray>
#include <random>

inline QColor color_to_rgb(const Color &color)
{
    double r = color.x();
    double g = color.y();
    double b = color.z();
    r = std::clamp(r, 0.0, 0.999999999);
    g = std::clamp(g, 0.0, 0.999999999);
    b = std::clamp(b, 0.0, 0.999999999);
    r = qSqrt(r);
    g = qSqrt(g);
    b = qSqrt(b);
    return QColor(static_cast<int>(r * 256),
                  static_cast<int>(g * 256),
                  static_cast<int>(b * 256));
}

inline Color rgb_to_color(const QColor &color)
{
    return Color(color.redF(), color.greenF(), color.blueF());
}

inline Color rgb_to_color(int r, int g, int b)
{
    return rgb_to_color(QColor(r, g, b));
}

inline Color normal_to_color(const QVector3D &normal)
{
    return 0.5 * (normal + Color(1.0, 1.0, 1.0));
}

inline double random_double()
{
    static std::uniform_real_distribution<double> dis(0.0, 1.0);
    static std::mt19937 gen{std::random_device{}()};
    return dis(gen);
}

inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max)
{
    return static_cast<int>(random_double(min, max));
}

inline QVector3D random_vec()
{
    return QVector3D(random_double(), random_double(), random_double());
}

inline QVector3D random_vec(double min, double max)
{
    return QVector3D(random_double(min, max),
                     random_double(min, max),
                     random_double(min, max));
}

inline QVector3D random_in_unit_sphere()
{
    while (true)
    {
        auto p = random_vec(-1, 1);
        if (p.lengthSquared() >= 1)
        {
            continue;
        }
        return p;
    }
}

inline QVector3D random_unit_vec()
{
    return random_in_unit_sphere().normalized();
}

inline QVector3D random_in_hemisphere(const QVector3D normal)
{
    auto in_unit_sphere = random_in_unit_sphere();
    return QVector3D::dotProduct(in_unit_sphere, normal) > 0 ? // 和法向同向
               in_unit_sphere
                                                             : -in_unit_sphere;
}

inline QVector3D random_in_unit_disk()
{
    while (true)
    {
        auto p = QVector3D(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.lengthSquared() >= 1)
        {
            continue;
        }
        return p;
    }
}

inline QVector3D reflect(const QVector3D &v, const QVector3D &n)
{
    return v - 2 * QVector3D::dotProduct(v, n) * n;
}

inline QVector3D refract(const QVector3D &unit_v, const QVector3D &n, double refraction_ratio)
{
    auto cos_theta = qMin(QVector3D::dotProduct(-unit_v, n), 1.0);
    QVector3D r_perp = refraction_ratio * (unit_v + cos_theta * n);
    QVector3D r_parallel = -sqrt(qAbs(1.0 - r_perp.lengthSquared())) * n;
    return r_perp + r_parallel;
}

inline QVector3D json_to_vec(const QJsonArray &array)
{
    return QVector3D(array[0].toDouble(),
                     array[1].toDouble(),
                     array[2].toDouble());
}

#endif // UTILS_H
