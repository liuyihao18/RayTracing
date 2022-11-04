#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H
#include "material.h"
#include "hittable.h"
#include "texture.h"
#include "color_texture.h"
#include "utils.h"
#include "double.h"

class Lambertian : public Material
{
public:
    Lambertian(const Color &color) : _texture(QSharedPointer<ColorTexture>::create(color)){};
    Lambertian(QSharedPointer<Texture> texture) : _texture(texture) {}

    virtual bool scatter(const Ray &ray_in, const HitRecord &rec,
                         Color &attenuation, Ray &ray_out) const override
    {
        // QVector3D scatter_direction = random_in_hemisphere(rec.n); // approximate
        // QVector3D scatter_direction = rec.n + random_in_unit_sphere(); // cos^3(\theta)分布
        QVector3D scatter_direction = rec.n + random_unit_vec(); // cos(\theta)分布
        if (eq(scatter_direction.length(), 0))
        {
            scatter_direction = rec.n;
        }
        ray_out = Ray(rec.p, scatter_direction);
        attenuation = _texture->get_color(rec.u, rec.v, rec.p);
        return true;
    }

private:
    QSharedPointer<Texture> _texture; // 纹理
};

#endif // LAMBERTIAN_H
