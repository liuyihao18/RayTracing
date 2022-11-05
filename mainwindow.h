#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "custom.h"
#include "scene.h"
#include "camera.h"
#include "renderer.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initialize();

private slots:
    void onProgressUpdated(double progress);
    void onErrorMsgReceived(QString errMsg);

    void on_importButton_clicked();
    void on_abortButton_clicked();

    void on_renderButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::MainWindow *ui;
    QImage _image;
    Scene _scene;
    Camera _camera;
    Renderer* _renderer;
};
#endif // MAINWINDOW_H
