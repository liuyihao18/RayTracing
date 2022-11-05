#include "renderer.h"
#include "camera.h"
#include "bvh_node.h"
#include "scene.h"
#include "material.h"
#include "utils.h"
#include <QImage>
#include <iostream>

Color ray_color(const Ray &ray, const Color &background, const Hittable &scene, int depth)
{
    if (depth <= 0)
    {
        return Color(0, 0, 0);
    }

    HitRecord rec;
    if (!scene.hit(ray, nearest, INFINITY, rec))
    {
        return background;
    }

    Ray scattered;
    Color attenuation;
    Color emitted = rec.material_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.material_ptr->scatter(ray, rec, attenuation, scattered))
    {
        return emitted;
    }

    return emitted + attenuation * ray_color(scattered, background, scene, depth - 1);
}

void Renderer::render(bool use_bvh)
{
    if (use_bvh)
    {
        BVHNode bvh(_scene.src());
        _render(_camera, _scene.background(), bvh, _image);
    }
    else
    {
        _render(_camera, _scene.background(), _scene.src(), _image);
    }
}

void Renderer::run()
{
    render(true);
}

void Renderer::_render(const Camera &camera, const Color &background, const Hittable &_scene, QImage &image)
{
    QVector<QVector<Color>> buffer(_image_width, QVector<Color>(_image_height, Color(0, 0, 0)));
    for (int s = 0; s < _n_samples; s++)
    {
        emit update_progress(s * 1.0 / _n_samples);
        for (int y = 0; y < _image_height; y++)
        {
            for (int x = 0; x < _image_width; x++)
            {
                auto u = (x + random_double()) / (_image_width - 1);
                auto v = (y + random_double()) / (_image_height - 1);
                Ray ray = camera.get_ray(u, v);
                buffer[x][y] += ray_color(ray, background, _scene, _max_depth);
                image.setPixelColor(QPoint(x, _image_height - y - 1), color_to_rgb(buffer[x][y] / (s + 1)));
            }
        }
    }
    emit update_progress(1.0);
}
