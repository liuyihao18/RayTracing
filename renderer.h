#ifndef RENDERER_H
#define RENDERER_H
#include "custom.h"

constexpr int n_threads = 4;
constexpr int n_samples = 50;
constexpr int max_depth = 50;
constexpr double nearest = 0.001;

class Renderer
{
public:
    Renderer(int image_height, double aspect_ratio) : _image_height(image_height), _image_width(image_height * aspect_ratio) {}
    void render(const Camera &camera, const Scene &scene, const QString &output_filename, bool use_bvh = true) const;

private:
    int _image_height;
    int _image_width;
    void _render(const Camera &camera, const Color &background, const Hittable &_scene, const QString &output_filename) const;
};

#endif // RENDERER_H
