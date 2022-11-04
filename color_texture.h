#ifndef COLOR_TEXTURE_H
#define COLOR_TEXTURE_H
#include "texture.h"

class ColorTexture : public Texture
{
public:
    ColorTexture() = default;
    ColorTexture(const Color &color) : _color(color) {}
    ColorTexture(double r, double g, double b) : _color(r, g, b) {}

    virtual Color get_color(double u, double v, const Point &p) const override
    {
        return _color;
    }

private:
    Color _color;
};

#endif // COLOR_TEXTURE_H
