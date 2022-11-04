#ifndef METAL_H
#define METAL_H
#include "material.h"
#include "hittable.h"
#include "utils.h"

class Metal : public Material
{
public:
    Metal(const Color &albedo, double fuzz) : _albedo(albedo), _fuzz(std::clamp(fuzz, 0.0, 1.0)) {}

    virtual bool scatter(const Ray &ray_in, const HitRecord &rec,
                         Color &attenuation, Ray &ray_out) const override
    {
        QVector3D reflect_direction = reflect(ray_in.direction(), rec.n);
        ray_out = Ray(rec.p, reflect_direction + _fuzz * random_in_unit_sphere());
        attenuation = _albedo;
        return QVector3D::dotProduct(ray_out.direction(), rec.n) > 0;
    }

private:
    Color _albedo; // 颜色衰减
    double _fuzz;  // 模糊度
};

#endif // METAL_H
