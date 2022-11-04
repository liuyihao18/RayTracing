#include "scene.h"
#include "sphere.h"
#include "arect.h"
#include "cube.h"
#include "triangle.h"
#include "mesh.h"
#include "glass.h"
#include "lambertian.h"
#include "metal.h"
#include "image_texture.h"
#include "light.h"

Scene Scene::test_scene_1()
{
    Scene scene(Color(0.2, 0.2, 0.2));

    /* ground */
    auto ground_material = QSharedPointer<Lambertian>::create(Color(0.8, 0.7, 0.4));
    auto ground = QSharedPointer<XZRect>::create(-10, 10, -10, 10, 0, ground_material);
    scene.add(ground);

    /* sphere */
    auto earth_texture = QSharedPointer<ImageTexture>::create("earth.jpg");
    auto center_material = QSharedPointer<Lambertian>::create(earth_texture);
    auto center = QSharedPointer<Sphere>::create(Point(0, 0.5, 0), 0.5, center_material);
    scene.add(center);

    auto left_material = QSharedPointer<Glass>::create(1.5);
    auto left_outer = QSharedPointer<Sphere>::create(Point(-1, 0.5, 0), 0.5, left_material);
    auto left_inner = QSharedPointer<Sphere>::create(Point(-1, 0.5, 0), -0.45, left_material);
    scene.add(left_outer);
    scene.add(left_inner);

    auto right_material = QSharedPointer<Metal>::create(Color(0.8, 0.6, 0.2), 0.1);
    auto right = QSharedPointer<Sphere>::create(Point(1, 0.5, 0), 0.5, right_material);
    scene.add(right);

    auto back_material = QSharedPointer<Lambertian>::create(Color(0.4, 0.7, 0.2));
    auto back = QSharedPointer<Sphere>::create(Point(-0.5, 0.25, -0.4), 0.25, back_material);
    scene.add(back);

    /* cube */
    auto container_texture = QSharedPointer<ImageTexture>::create("container.jpg");
    auto container_material = QSharedPointer<Lambertian>::create(container_texture);
    auto container = QSharedPointer<Cube>::create(Point(0, 0, 0), Point(1, 1, 1), container_material);
    QMatrix4x4 container_model;
    container_model.translate(QVector3D(0.3, 0, 0.7));
    container_model.scale(0.5);
    scene.add(QSharedPointer<Transform>::create(container, container_model));

    /* light */
    auto light_material = QSharedPointer<Light>::create(Color(1.0, 1.0, 1.0), 5.0);
    auto light = QSharedPointer<Sphere>::create(Point(-0.5, 1.5, 0), 0.1, light_material);
    scene.add(light);

    /* mirror */
    auto mirror_material = QSharedPointer<Metal>::create(Color(1, 1, 1), 0.0);
    auto mirror = QSharedPointer<XYRect>::create(-1.5, 1.5, 0, 1.75, -1, mirror_material);
    auto mirror_border_material = QSharedPointer<Lambertian>::create(Color(0.6, 0.8, 0.7));
    auto mirror_border = QSharedPointer<XYRect>::create(-1.505, 1.505, 0, 1.755, -1.005, mirror_border_material);
    QMatrix4x4 mirror_model;
    mirror_model.rotate(-15, QVector3D(0, 1, 0));
    scene.add(mirror, mirror_model);
    scene.add(mirror_border, mirror_model);

    /* triangle */
    auto triangle_material = QSharedPointer<Metal>::create(Color(0.8, 0.4, 0.3), 0.1);
    auto triangle = QSharedPointer<Triangle>::create(Point(2.5, 0, 1), Point(2, 1.5, 1), Point(2.5, 0, -1), triangle_material);
    scene.add(triangle);

    return scene;
}

Scene Scene::test_scene_2()
{
    Scene scene(Color(0.0, 0.0, 0.0));

    /* ground */
    auto ground_material = QSharedPointer<Lambertian>::create(Color(0.8, 0.7, 0.4));
    auto ground = QSharedPointer<XZRect>::create(-10, 10, -10, 10, 0, ground_material);
    scene.add(ground);

    /* bunny */
    auto bunny_material = QSharedPointer<Lambertian>::create(Color(0.9, 0.9, 0.9));
    auto bunny = QSharedPointer<Mesh>::create("./bunny.obj", bunny_material);
    QMatrix4x4 bunny_model;
    bunny_model.scale(8.0);
    scene.add(bunny, bunny_model);

    /* light */
    auto light_texture = QSharedPointer<ImageTexture>::create("./sky.jpg");
    auto light_material = QSharedPointer<Light>::create(light_texture, 1.0);
    auto light = QSharedPointer<Sphere>::create(Point(0, 0, 0), 1000, light_material);
    scene.add(light);

    return scene;
}

Scene Scene::final_scene()
{

    Scene scene(Color(0.0, 0.0, 0.0));

    /* ground */
    auto ground_material = QSharedPointer<Lambertian>::create(Color(0.8, 0.7, 0.4));
    auto ground = QSharedPointer<XZRect>::create(-10, 10, -10, 10, 0, ground_material);
    scene.add(ground);

    /* sphere */
    auto earth_texture = QSharedPointer<ImageTexture>::create("earth.jpg");
    auto center_material = QSharedPointer<Lambertian>::create(earth_texture);
    auto center = QSharedPointer<Sphere>::create(Point(0, 0.5, 0), 0.5, center_material);
    scene.add(center);

    auto left_material = QSharedPointer<Glass>::create(1.5);
    auto left_outer = QSharedPointer<Sphere>::create(Point(-1, 0.5, 0), 0.5, left_material);
    auto left_inner = QSharedPointer<Sphere>::create(Point(-1, 0.5, 0), -0.45, left_material);
    scene.add(left_outer);
    scene.add(left_inner);

    auto right_material = QSharedPointer<Metal>::create(Color(0.8, 0.6, 0.2), 0.1);
    auto right = QSharedPointer<Sphere>::create(Point(1, 0.5, 0), 0.5, right_material);
    scene.add(right);

    auto back_material = QSharedPointer<Lambertian>::create(Color(0.4, 0.7, 0.2));
    auto back = QSharedPointer<Sphere>::create(Point(-0.5, 0.25, -0.4), 0.25, back_material);
    scene.add(back);

    /* cube */
    auto container_texture = QSharedPointer<ImageTexture>::create("container.jpg");
    auto container_material = QSharedPointer<Lambertian>::create(container_texture);
    auto container = QSharedPointer<Cube>::create(Point(0, 0, 0), Point(1, 1, 1), container_material);
    QMatrix4x4 container_model;
    container_model.translate(QVector3D(0.3, 0, 0.7));
    container_model.scale(0.5);
    scene.add(QSharedPointer<Transform>::create(container, container_model));

    /* light */
    auto light_material = QSharedPointer<Light>::create(Color(1.0, 1.0, 1.0), 5.0);
    auto light = QSharedPointer<Sphere>::create(Point(-0.5, 1.35, 0.25), 0.1, light_material);
    scene.add(light);

    /* light */
    auto ambient_light_texture = QSharedPointer<ImageTexture>::create("./sky.jpg");
    auto ambient_light_material = QSharedPointer<Light>::create(ambient_light_texture, 1.0);
    auto ambient_light = QSharedPointer<Sphere>::create(Point(0, 0, 0), 1000, ambient_light_material);
    scene.add(ambient_light);

    /* mirror */
    auto mirror_material = QSharedPointer<Metal>::create(Color(1, 1, 1), 0.0);
    auto mirror = QSharedPointer<XYRect>::create(-1.5, 1.5, 0, 1.75, -1, mirror_material);
    auto mirror_border_material = QSharedPointer<Lambertian>::create(Color(0.6, 0.8, 0.7));
    auto mirror_border = QSharedPointer<XYRect>::create(-1.505, 1.505, 0, 1.755, -1.005, mirror_border_material);
    QMatrix4x4 mirror_model;
    mirror_model.rotate(-15, QVector3D(0, 1, 0));
    scene.add(mirror, mirror_model);
    scene.add(mirror_border, mirror_model);

    /* triangle */
    auto triangle_material = QSharedPointer<Metal>::create(Color(0.8, 0.4, 0.3), 0.1);
    auto triangle = QSharedPointer<Triangle>::create(Point(2.5, 0, 1), Point(2, 1.5, 1), Point(2.5, 0, -1), triangle_material);
    scene.add(triangle);

    /* bunny */
    auto bunny_material = QSharedPointer<Lambertian>::create(rgb_to_color(255, 251, 240));
    auto bunny = QSharedPointer<Mesh>::create("./bunny.obj", bunny_material);
    QMatrix4x4 bunny_model;
    bunny_model.translate(QVector3D(-0.25, 0, 0.88));
    bunny_model.scale(3.0);
    scene.add(bunny, bunny_model);

    return scene;
}
