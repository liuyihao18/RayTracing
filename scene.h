#ifndef SCENE_H
#define SCENE_H
#include "hittable_list.h"
#include "transform.h"
#include <QVector>

class Scene
{
public:
    static Scene createFromJson(const QString &path, bool *ok = nullptr);
    static Scene test_scene_1();
    static Scene test_scene_2();
    static Scene test_scene_3();

public:
    Scene() = default;
    Scene(const Color &background);
    Scene(const QString& background_image);

    void add(QSharedPointer<Hittable> object) { _src.add(object); }
    void add(QSharedPointer<Hittable> object, const QMatrix4x4 &model) { _src.add(QSharedPointer<Transform>::create(object, model)); }
    void clear() { _src.clear(); }
    HittableList src() const noexcept { return _src; };

private:
    HittableList _src;
};

#endif // SCENE_H
