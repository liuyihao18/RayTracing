#ifndef MATERIAL_H
#define MATERIAL_H
#include "custom.h"

class Material
{
public:
    virtual bool scatter(const Ray &ray_in, const HitRecord &rec,
                         Color &attenuation, Ray &ray_out) const = 0;

    virtual Color emitted(double u, double v, const Point &p) const
    {
        return Color(0, 0, 0);
    }
};

#endif // MATERIAL_H
