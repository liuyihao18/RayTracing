#ifndef DIELECTRIC_H
#define DIELECTRIC_H
#include "material.h"
#include "hittable.h"
#include "utils.h"

class Glass : public Material
{
public:
    Glass(double ir) : _ir(ir) {}

    virtual bool scatter(const Ray &ray_in, const HitRecord &rec,
                         Color &attenuation, Ray &ray_out) const override
    {
        attenuation = Color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / _ir) : _ir;
        QVector3D in_direction = ray_in.direction().normalized();
        double cos_theta = qMin(QVector3D::dotProduct(-in_direction, rec.n), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        QVector3D out_direction;
        if (refraction_ratio * sin_theta > 1.0 || reflectance(cos_theta, refraction_ratio) > random_double())
        {
            out_direction = reflect(in_direction, rec.n);
        }
        else
        {
            out_direction = refract(in_direction, rec.n, refraction_ratio);
        }
        ray_out = Ray(rec.p, out_direction);
        return true;
    }

private:
    double _ir; // 折射率

private:
    static double reflectance(double cosine, double ref_idx)
    {
        // 使用Schlick公式近似
        double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * qPow((1 - cosine), 5);
    }
};

#endif // DIELECTRIC_H
