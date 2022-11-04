#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H
#include "texture.h"
#include <QImage>

class ImageTexture : public Texture
{
public:
    ImageTexture() {}
    ImageTexture(const QString &filename) : _image(filename) {}

    virtual Color get_color(double u, double v, const Point &p) const override
    {
        if (_image.isNull())
        {
            return Color(0, 1, 1);
        }
        u = std::clamp(u, 0.0, 1.0);
        v = 1.0 - std::clamp(v, 0.0, 1.0);

        int i = static_cast<int>(u * _image.width());
        int j = static_cast<int>(v * _image.height());

        i = std::clamp(i, 0, _image.width() - 1);
        j = std::clamp(j, 0, _image.height() - 1);;

        QColor color = _image.pixelColor(i, j);
        double color_scale = 1.0 / 255.0;

        return Color(color.red() * color_scale, color.green() * color_scale, color.blue() * color_scale);
    }

private:
    QImage _image;
};

#endif // IMAGE_TEXTURE_H
