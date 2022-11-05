#include "renderer.h"
#include "camera.h"
#include "bvh_node.h"
#include "scene.h"
#include "material.h"
#include "utils.h"
#include "gui_handler.h"
#include <QImage>
#include <iostream>

Color ray_color(const Ray &ray, const Hittable &scene, int depth)
{
    if (depth <= 0)
    {
        return Color(0, 0, 0);
    }

    HitRecord rec;
    if (!scene.hit(ray, nearest, INFINITY, rec))
    {
        return Color(0, 0, 0);
    }

    Ray scattered;
    Color attenuation;
    Color emitted = rec.material_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.material_ptr->scatter(ray, rec, attenuation, scattered))
    {
        return emitted;
    }

    return emitted + attenuation * ray_color(scattered, scene, depth - 1);
}

void Renderer::render(const Camera &camera, const Scene &scene, QImage &image, bool use_bvh)
{
    if (use_bvh)
    {
        BVHNode bvh(scene.src());
        _render(camera, bvh, image);
    }
    else
    {
        _render(camera, scene.src(), image);
    }
}

void Renderer::_render(const Camera &camera, const Hittable &_scene, QImage &image)
{
    QVector<QVector<Color>> buffer(_image_width, QVector<Color>(_image_height, Color(0, 0, 0)));
    int rendered_rows = 0;
    for (int s = 0; s < _n_samples; s++)
    {
        for (int y = 0; y < _image_height; y++)
        {
            GUIHandler::Inst()->updateProgress((rendered_rows * 1.0) / (_n_samples * _image_height));
            for (int x = 0; x < _image_width; x++)
            {
                auto u = (x + random_double()) / (_image_width - 1);
                auto v = (y + random_double()) / (_image_height - 1);
                Ray ray = camera.get_ray(u, v);
                buffer[x][y] += ray_color(ray, _scene, _max_depth);
                image.setPixelColor(QPoint(x, _image_height - y - 1), color_to_rgb(buffer[x][y] / (s + 1)));
            }
            rendered_rows++;
        }
    }
    GUIHandler::Inst()->updateProgress(1.0);
}
