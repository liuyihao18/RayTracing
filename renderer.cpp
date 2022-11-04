#include "renderer.h"
#include "camera.h"
#include "bvh_node.h"
#include "scene.h"
#include "material.h"
#include "utils.h"
#include "double.h"
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

inline void update_progress(double progress)
{
    int barWidth = 70;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << static_cast<int>(progress * 100.0) << " %";
    if (ne(progress, 1.0))
    {
        std::cout << "\r";
        std::cout.flush();
    }
    else
    {
        std::cout << "\n";
    }
};

void Renderer::render(const Camera &camera, const Scene &scene, const QString &output_filename, bool use_bvh) const
{
    if (use_bvh)
    {
        BVHNode bvh(scene.src());
        _render(camera, scene.background(), bvh, output_filename);
    }
    else
    {
        _render(camera, scene.background(), scene.src(), output_filename);
    }
}

void Renderer::_render(const Camera &camera, const Color &background, const Hittable &_scene, const QString &output_filename) const
{
    QImage output(_image_width, _image_height, QImage::Format_RGB888);

    QVector<QVector<Color>> buffer(_image_width, QVector<Color>(_image_height, Color(0, 0, 0)));
    for (int s = 0; s < n_samples; s++)
    {
        update_progress(s * 1.0 / n_samples);
        for (int y = 0; y < _image_height; y++)
        {
            for (int x = 0; x < _image_width; x++)
            {
                auto u = (x + random_double()) / (_image_width - 1);
                auto v = (y + random_double()) / (_image_height - 1);
                Ray ray = camera.get_ray(u, v);
                buffer[x][y] += ray_color(ray, background, _scene, max_depth);
            }
        }
    }
    update_progress(1.0);
    for (int y = 0; y < _image_height; y++)
    {
        for (int x = 0; x < _image_width; x++)
        {
            // QImage是以左上角作为坐标原点，需要翻转
            output.setPixelColor(QPoint(x, _image_height - y - 1), color_to_rgb(buffer[x][y] / n_samples));
        }
    }

    output.save(output_filename);
}
