#include "camera.h"
#include "gui_handler.h"
#include "utils.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

Camera Camera::createFromJson(const QString &path, bool *ok)
{
    QFile jsonFile(path);
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        GUIHandler::Inst()->warn("无法打开场景文件：" + jsonFile.fileName());
        if (ok)
        {
            *ok = false;
        }
        return Camera();
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
        return Camera();
    }

    if (ok)
    {
        *ok = true;
    }

    Point eye = Point(0, 0, 1);
    Point lookat = Point(0, 1, 0);
    Point up = Point(0, 1, 0);
    float vfov = 60.0;

    auto json = jsonDoc.object();
    if (!json.contains("camera"))
    {
        GUIHandler::Inst()->info("找不到相机配置：" + jsonFile.fileName());
        return Camera(eye, lookat, up, vfov);
    }

    auto camera = json.value("camera").toObject();
    if (camera.contains("eye"))
    {
        auto eye_json = camera.value("eye");
        if (!eye_json.isArray() || eye_json.toArray().size() != 3)
        {
            GUIHandler::Inst()->info("camera.eye");
        }
        else
        {
            eye = json_to_vec(eye_json.toArray());
        }
    }
    if (camera.contains("lookat"))
    {
        auto lookat_json = camera.value("lookat");
        if (!lookat_json.isArray() || lookat_json.toArray().size() != 3)
        {
            GUIHandler::Inst()->info("camera.lookat");
        }
        else
        {
            lookat = json_to_vec(lookat_json.toArray());
        }
    }
    if (camera.contains("up"))
    {
        auto up_json = camera.value("up");
        if (!up_json.isArray() || up_json.toArray().size() != 3)
        {
            GUIHandler::Inst()->info("camera.up");
        }
        else
        {
            up = json_to_vec(up_json.toArray());
        }
    }
    if (camera.contains("vfov"))
    {
        auto vfov_json = camera.value("vfov");
        if (!vfov_json.isDouble())
        {
            GUIHandler::Inst()->info("camera.vfov");
        }
        else
        {
            vfov = camera.value("vfov").toDouble();
        }
    }

    return Camera(eye, lookat, up, vfov);
}

Camera::Camera(const Point &eye, const Point &lookat, const QVector3D &up, double vfov)
{
    auto theta = qDegreesToRadians(vfov);
    auto h = qTan(theta / 2.0);
    auto viewport_height = 2.0 * h;
    auto viewport_width = viewport_height * aspect_ratio;

    _w = (eye - lookat).normalized();     // w轴
    _u = QVector3D::crossProduct(up, _w); // u轴
    _v = QVector3D::crossProduct(_w, _u); // v轴

    _origin = eye;
    _horizontal = viewport_width * _u; // 投影平面水平方向（u轴）
    _vertical = viewport_height * _v;  // 投影平面竖直方向（v轴）
    _left_bottom_corner = _origin - _horizontal / 2 - _vertical / 2 - _w;
}

Ray Camera::get_ray(double s, double t) const
{
    return Ray(
        _origin,
        _left_bottom_corner + s * _horizontal + t * _vertical - _origin);
}
