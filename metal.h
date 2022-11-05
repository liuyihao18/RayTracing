#ifndef METAL_H
#define METAL_H
#include "material.h"
#include "texture.h"
#include "color_texture.h"
#include "hittable.h"
#include "utils.h"

class Metal : public Material
{
public:
    Metal(const Color &color, double fuzz)
        : _texture(QSharedPointer<ColorTexture>::create(color)), _fuzz(std::clamp(fuzz, 0.0, 1.0)) {}
    Metal(QSharedPointer<Texture> texture, double fuzz)
        : _texture(texture), _fuzz(fuzz) {}

    virtual bool scatter(const Ray &ray_in, const HitRecord &rec,
                         Color &attenuation, Ray &ray_out) const override
    {
        QVector3D reflect_direction = reflect(ray_in.direction(), rec.n);
        ray_out = Ray(rec.p, reflect_direction + _fuzz * random_in_unit_sphere());
        attenuation = _texture->get_color(rec.u, rec.v, rec.p);
        return QVector3D::dotProduct(ray_out.direction(), rec.n) > 0;
    }

private:
    QSharedPointer<Texture> _texture; // 纹理
    double _fuzz;  // 模糊度
};

#endif // METAL_H
