#ifndef CUSTOM_H
#define CUSTOM_H
#include <QSharedPointer>
#include <QVector3D>
#include <QMatrix4x4>

constexpr double aspect_ratio = 16.0 / 9.0;
const double PI = qAtan(1) * 4;

using Point = QVector3D;
using Color = QVector3D;

class Ray;
class Camera;
class Renderer;

struct HitRecord;
class Hittable;
class HittableList;
class Scene;
class Sphere;
class MovingSphere;
class XYRect;
class XZRect;
class YZRect;
class ConstantMedium;

class Translate;
class RotateY;
class Transform;

class Material;
class Lambertian;
class Metal;
class Glass;

class AABB;
class BVHNode;

class Texture;
class ColorTexture;
class CheckerTexture;
class PerlinTexture;
class ImageTexture;

class Perlin;

#endif // CUSTOM_H
