#ifndef LIGHT_H
#define LIGHT_H
#include "material.h"
#include "texture.h"
#include "color_texture.h"

class Light : public Material
{
public:
    Light(const Color &color, double intensity) : _texture(QSharedPointer<ColorTexture>::create(color)), _intensity(intensity) {}
    Light(QSharedPointer<Texture> texture, double intensity) : _texture(texture), _intensity(intensity) {}

    virtual bool scatter(const Ray &ray_in, const HitRecord &rec,
                         Color &attenuation, Ray &ray_out) const override
    {
        return false;
    }

    virtual Color emitted(double u, double v, const Point &p) const override
    {
        return _intensity * _texture->get_color(u, v, p);
    }

private:
    QSharedPointer<Texture> _texture; // 纹理
    double _intensity; // 光照强度
};

#endif // LIGHT_H
