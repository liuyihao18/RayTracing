#ifndef SCENE_H
#define SCENE_H
#include "hittable_list.h"
#include "transform.h"
#include <QVector>

class Scene
{
public:
    static Scene test_scene_1();
    static Scene test_scene_2();
    static Scene final_scene();

public:
    Scene() = default;
    Scene(const Color &background) : _background(background) {}

    Color background() const noexcept { return _background; }

    void add(QSharedPointer<Hittable> object) { _src.add(object); }
    void add(QSharedPointer<Hittable> object, const QMatrix4x4 &model) { _src.add(QSharedPointer<Transform>::create(object, model)); }
    void clear() { _src.clear(); }
    HittableList src() const noexcept { return _src; };

private:
    Color _background;
    HittableList _src;
};

#endif // SCENE_H
