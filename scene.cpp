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
#include "gui_handler.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

Scene Scene::createFromJson(const QString &path, bool *ok)
{
    QFile jsonFile(path);
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        GUIHandler::Inst()->warn("无法打开场景文件：" + jsonFile.fileName());
        if (ok)
        {
            *ok = false;
        }
        return Scene();
    }

    auto data = jsonFile.readAll();
    jsonFile.close();

    QJsonParseError jsonError;
    auto jsonDoc = QJsonDocument::fromJson(data, &jsonError);
    if (jsonError.error != QJsonParseError::NoError)
    {
        GUIHandler::Inst()->warn("场景文件格式错误：" + jsonFile.fileName());
        if (ok)
        {
            *ok = false;
        }
        return Scene();
    }

    if (ok)
    {
        *ok = true;
    }

    auto json = jsonDoc.object();
    Scene scene(Color(0.1, 0.15, 0.2));
    if (json.contains("background"))
    {
        auto background_json = json.value("background");
        if (background_json.isArray())
        {
            if (background_json.toArray().size() != 3)
            {
                GUIHandler::Inst()->info("background");
            }
            scene = Scene(json_to_vec(background_json.toArray()));
        }
        else if (background_json.isString())
        {
            scene = Scene(background_json.toString());
        }
        else
        {
            GUIHandler::Inst()->info("background");
        }
    }

    if (json.contains("objects"))
    {
        auto objects_json = json.value("objects");
        if (!objects_json.isArray())
        {
            GUIHandler::Inst()->info("objects");
        }
        auto object = objects_json.toArray();
        for (int i = 0; i < object.size(); i++)
        {
            auto object_json = objects_json[i];
            if (!object_json.isObject())
            {
                GUIHandler::Inst()->info("objects[" + QString::number(i) + "]");
                continue;
            }
            auto object = object_json.toObject();

            /* texture */
            QSharedPointer<Texture> texture_ptr = QSharedPointer<ColorTexture>::create(Color(0.5, 0.5, 0.5));
            ;
            if (object.contains("texture"))
            {
                auto texture_json = object.value("texture");
                if (texture_json.isArray() && texture_json.toArray().size() == 3)
                {
                    texture_ptr = QSharedPointer<ColorTexture>::create(json_to_vec(texture_json.toArray()));
                }
                else if (texture_json.isString())
                {
                    texture_ptr = QSharedPointer<ImageTexture>::create(texture_json.toString());
                }
            }

            /* material */
            QSharedPointer<Material> material_ptr = QSharedPointer<Lambertian>::create(texture_ptr);
            if (object.contains("material"))
            {
                auto material_json = object.value("material");
                if (material_json.isObject())
                {
                    auto material = material_json.toObject();
                    if (material.contains("type"))
                    {
                        auto type = material.value("type").toString("lambertian");
                        if (type == "lambertian")
                        {
                            material_ptr = QSharedPointer<Lambertian>::create(texture_ptr);
                        }
                        else if (type == "glass")
                        {
                            double ratio = 1.5;
                            if (material.contains("ratio"))
                            {
                                ratio = material.value("ratio").toDouble(ratio);
                            }
                            material_ptr = QSharedPointer<Glass>::create(ratio);
                        }
                        else if (type == "metal")
                        {
                            double fuzz = 0.0;
                            if (material.contains("fuzz"))
                            {
                                fuzz = material.value("fuzz").toDouble(fuzz);
                            }
                            material_ptr = QSharedPointer<Metal>::create(texture_ptr, fuzz);
                        }
                    }
                }
            }

            /* type */
            QString type = "sphere";
            if (object.contains("type"))
            {
                type = object.value("type").toString(type);
            }
            QSharedPointer<Hittable> ptr;
            if (type == "sphere")
            {
                Point center(0, 0, 0);
                double radius = 1.0;
                if (object.contains("geometric"))
                {
                    auto geometric_json = object.value("geometric");
                    if (geometric_json.isObject())
                    {
                        auto geometric = geometric_json.toObject();
                        if (geometric.contains("center"))
                        {
                            center = json_to_vec(geometric.value("center").toArray({center[0], center[1], center[2]}));
                        }
                        if (geometric.contains("radius"))
                        {
                            radius = geometric.value("radius").toDouble(radius);
                        }
                    }
                }
                ptr = QSharedPointer<Sphere>::create(center, radius, material_ptr);
            }
            else if (type == "xz_rect")
            {
                double x0 = -1;
                double x1 = 1;
                double z0 = -1;
                double z1 = 1;
                double y = 0;
                if (object.contains("geometric"))
                {
                    auto geometric_json = object.value("geometric");
                    if (geometric_json.isObject())
                    {
                        auto geometric = geometric_json.toObject();
                        if (geometric.contains("x0"))
                        {
                            x0 = geometric.value("x0").toDouble(x0);
                        }
                        if (geometric.contains("x1"))
                        {
                            x1 = geometric.value("x1").toDouble(x1);
                        }
                        if (geometric.contains("z0"))
                        {
                            z0 = geometric.value("z0").toDouble(z0);
                        }
                        if (geometric.contains("z1"))
                        {
                            z1 = geometric.value("z1").toDouble(z1);
                        }
                        if (geometric.contains("y"))
                        {
                            y = geometric.value("y").toDouble(y);
                        }
                    }
                }
                ptr = QSharedPointer<XZRect>::create(x0, x1, z0, z1, y, material_ptr);
            }
            else if (type == "xy_rect")
            {
                double x0 = -1;
                double x1 = 1;
                double y0 = -1;
                double y1 = 1;
                double z = 0;
                if (object.contains("geometric"))
                {
                    auto geometric_json = object.value("geometric");
                    if (geometric_json.isObject())
                    {
                        auto geometric = geometric_json.toObject();
                        if (geometric.contains("x0"))
                        {
                            x0 = geometric.value("x0").toDouble(x0);
                        }
                        if (geometric.contains("x1"))
                        {
                            x1 = geometric.value("x1").toDouble(x1);
                        }
                        if (geometric.contains("y0"))
                        {
                            y0 = geometric.value("y0").toDouble(y0);
                        }
                        if (geometric.contains("y1"))
                        {
                            y1 = geometric.value("y1").toDouble(y1);
                        }
                        if (geometric.contains("z"))
                        {
                            z = geometric.value("z").toDouble(z);
                        }
                    }
                }
                ptr = QSharedPointer<XYRect>::create(x0, x1, y0, y1, z, material_ptr);
            }
            else if (type == "yz_rect")
            {
                double y0 = -1;
                double y1 = 1;
                double z0 = -1;
                double z1 = 1;
                double x = 0;
                if (object.contains("geometric"))
                {
                    auto geometric_json = object.value("geometric");
                    if (geometric_json.isObject())
                    {
                        auto geometric = geometric_json.toObject();
                        if (geometric.contains("y0"))
                        {
                            y0 = geometric.value("y0").toDouble(y0);
                        }
                        if (geometric.contains("y1"))
                        {
                            y1 = geometric.value("y1").toDouble(y1);
                        }
                        if (geometric.contains("z0"))
                        {
                            z0 = geometric.value("z0").toDouble(z0);
                        }
                        if (geometric.contains("z1"))
                        {
                            z1 = geometric.value("z1").toDouble(z1);
                        }
                        if (geometric.contains("x"))
                        {
                            x = geometric.value("x").toDouble(x);
                        }
                    }
                }
                ptr = QSharedPointer<XZRect>::create(y0, y1, z0, z1, x, material_ptr);
            }
            else if (type == "cube")
            {
                Point min(0.0, 0.0, 0.0);
                Point max(1.0, 1.0, 1.0);
                if (object.contains("geometric"))
                {
                    auto geometric_json = object.value("geometric");
                    if (geometric_json.isObject())
                    {
                        auto geometric = geometric_json.toObject();
                        if (geometric.contains("min"))
                        {
                            min = json_to_vec(geometric.value("min").toArray({min[0], min[1], min[2]}));
                        }
                        if (geometric.contains("max"))
                        {
                            max = json_to_vec(geometric.value("max").toArray({max[0], max[1], max[2]}));
                        }
                    }
                }
                ptr = QSharedPointer<Cube>::create(min, max, material_ptr);
            }
            else if (type == "triangle")
            {
                Point p1(1.0, 0.0, 0.0);
                Point p2(0.0, 1.0, 0.0);
                Point p3(0.0, 0.0, 1.0);
                if (object.contains("geometric"))
                {
                    auto geometric_json = object.value("geometric");
                    if (geometric_json.isObject())
                    {
                        auto geometric = geometric_json.toObject();
                        if (geometric.contains("p1"))
                        {
                            p1 = json_to_vec(geometric.value("p1").toArray({p1[0], p1[1], p1[2]}));
                        }
                        if (geometric.contains("p2"))
                        {
                            p2 = json_to_vec(geometric.value("p2").toArray({p2[0], p2[1], p2[2]}));
                        }
                        if (geometric.contains("p3"))
                        {
                            p3 = json_to_vec(geometric.value("p3").toArray({p3[0], p3[1], p3[2]}));
                        }
                    }
                    ptr = QSharedPointer<Triangle>::create(p1, p2, p3, material_ptr);
                }
            }
            else if (type == "mesh")
            {
                if (object.contains("file"))
                {
                    auto file = object.value("file").toString();
                    bool ok = false;
                    ptr = QSharedPointer<Mesh>::create(file, material_ptr, &ok);
                    if (!ok)
                    {
                        ptr.clear();
                    }
                }
            }
            if (!ptr.isNull())
            {
                if (object.contains("transform"))
                {
                    auto transform_json = object.value("transform");
                    if (transform_json.isObject())
                    {
                        auto transform = transform_json.toObject();
                        QMatrix4x4 matrix;
                        if (transform.contains("translate"))
                        {
                            auto translate = json_to_vec(transform.value("translate").toArray({0.0, 0.0, 0.0}));
                            matrix.translate(translate);
                        }
                        if (transform.contains("rotate_x"))
                        {
                            auto rotate_x = transform.value("rotate_x").toDouble(0.0);
                            matrix.rotate(rotate_x, QVector3D(1, 0, 0));
                        }
                        if (transform.contains("rotate_y"))
                        {
                            auto rotate_y = transform.value("rotate_y").toDouble(0.0);
                            matrix.rotate(rotate_y, QVector3D(0, 1, 0));
                        }
                        if (transform.contains("rotate_z"))
                        {
                            auto rotate_z = transform.value("rotate_z").toDouble(0.0);
                            matrix.rotate(rotate_z, QVector3D(0, 0, 1));
                        }
                        if (transform.contains("scale"))
                        {
                            auto scale = transform.value("scale").toDouble(0.0);
                            matrix.scale(scale);
                        }
                        scene.add(ptr, matrix);
                    }
                    else
                    {
                        scene.add(ptr);
                    }
                }
                else
                {
                    scene.add(ptr);
                }
            }
        }
        return scene;
    }

    return scene;
}

Scene Scene::test_scene_1()
{
    Scene scene(Color(0.2, 0.2, 0.2));

    /* ground */
    auto ground_material = QSharedPointer<Lambertian>::create(Color(0.8, 0.7, 0.4));
    auto ground = QSharedPointer<XZRect>::create(-10, 10, -10, 10, 0, ground_material);
    scene.add(ground);

    /* sphere */
    auto earth_texture = QSharedPointer<ImageTexture>::create("./texture/earth.jpg");
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
    auto container_texture = QSharedPointer<ImageTexture>::create("./texture/container.jpg");
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
    Scene scene("./texture/sky.jpg");

    /* ground */
    auto ground_material = QSharedPointer<Lambertian>::create(Color(0.8, 0.7, 0.4));
    auto ground = QSharedPointer<XZRect>::create(-10, 10, -10, 10, 0, ground_material);
    scene.add(ground);

    /* bunny */
    auto bunny_material = QSharedPointer<Lambertian>::create(Color(0.9, 0.9, 0.9));
    auto bunny = QSharedPointer<Mesh>::create("./model/bunny.obj", bunny_material);
    QMatrix4x4 bunny_model;
    bunny_model.scale(8.0);
    scene.add(bunny, bunny_model);

    return scene;
}

Scene Scene::test_scene_3()
{

    Scene scene("./texture/sky.jpg");

    /* ground */
    auto ground_material = QSharedPointer<Lambertian>::create(Color(0.8, 0.7, 0.4));
    auto ground = QSharedPointer<XZRect>::create(-10, 10, -10, 10, 0, ground_material);
    scene.add(ground);

    /* sphere */
    auto earth_texture = QSharedPointer<ImageTexture>::create("./texture/earth.jpg");
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
    auto container_texture = QSharedPointer<ImageTexture>::create("./texture/container.jpg");
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
    auto bunny = QSharedPointer<Mesh>::create("./model/bunny.obj", bunny_material);
    QMatrix4x4 bunny_model;
    bunny_model.translate(QVector3D(-0.25, 0, 0.88));
    bunny_model.scale(3.0);
    scene.add(bunny, bunny_model);

    return scene;
}

Scene::Scene(const Color &background)
{
    auto ambient_light_texture = QSharedPointer<ColorTexture>::create(background);
    auto ambient_light_material = QSharedPointer<Light>::create(ambient_light_texture, 1.0);
    auto ambient_light = QSharedPointer<Sphere>::create(Point(0, 0, 0), 10000, ambient_light_material);
    add(ambient_light);
}

Scene::Scene(const QString &background_image)
{
    auto ambient_light_texture = QSharedPointer<ImageTexture>::create(background_image);
    auto ambient_light_material = QSharedPointer<Light>::create(ambient_light_texture, 1.0);
    auto ambient_light = QSharedPointer<Sphere>::create(Point(0, 0, 0), 10000, ambient_light_material);
    add(ambient_light);
}
