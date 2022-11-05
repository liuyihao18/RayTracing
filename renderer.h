#ifndef RENDERER_H
#define RENDERER_H
#include "custom.h"
#include <QThread>

constexpr double nearest = 0.001;

class Renderer : public QThread
{
    Q_OBJECT

signals:
    void update_progress(double progress);

public:
    Renderer(const Camera &camera, const Scene &scene, QImage &image, int image_height, int n_samples, int max_depth)
        : _camera(camera), _scene(scene), _image(image),
          _image_height(image_height), _image_width(image_height * aspect_ratio),
          _n_samples(n_samples), _max_depth(max_depth)
    {
    }

    void render(bool use_bvh = true);
    void run() override;

private:
    const Camera &_camera;
    const Scene &_scene;
    QImage &_image;
    int _image_height;
    int _image_width;
    int _n_samples;
    int _max_depth;

    void _render(const Camera &camera, const Hittable &_scene, const Color &background, QImage &image);
};

#endif // RENDERER_H
