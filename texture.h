#ifndef TEXTURE_H
#define TEXTURE_H
#include "custom.h"

class Texture
{
public:
    virtual Color get_color(double u, double v, const Point &p) const = 0;
};

#endif // TEXTURE_H
