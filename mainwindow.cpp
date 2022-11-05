#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "scene.h"
#include "camera.h"
#include "renderer.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), _renderer(nullptr)
{
    ui->setupUi(this);
    initialize();
}

MainWindow::~MainWindow()
{
    if (_renderer) {
        delete _renderer;
    }
    delete ui;
}

void MainWindow::initialize()
{
    ui->progressBar->setValue(0);

    QStringList items;
    items << QString("120") << QString("225") << QString("360") << QString("480") << QString("640") << QString("720") << QString("1080") << QString("1600");
    ui->imageHeight->addItems(items);

    ui->nSamples->setMinimum(1);
    ui->nSamples->setMaximum(10000);
    ui->nSamples->setValue(10);

    ui->maxDepth->setMinimum(1);
    ui->maxDepth->setMaximum(100);
    ui->maxDepth->setValue(50);
}


void MainWindow::on_importButton_clicked()
{
    if (_renderer) {
        if (!_renderer->isFinished()) {
            return;
        }
        delete _renderer;
    }

    int image_height = ui->imageHeight->currentText().toInt();
    int image_width = image_height * aspect_ratio;
    _image = QImage(image_width, image_height, QImage::Format_RGB888);

    /* World */
    _scene = Scene::final_scene();

    /* Camera */
    Point eye(-2, 1.11, 2), lookat(0, 0.88, 0), up(0, 1, 0);
    auto vfov = 60.0;
    _camera = Camera(eye, lookat, up, vfov);

    /* Renderer */
    int n_samples = ui->nSamples->value();
    int max_depth = ui->maxDepth->value();
    _renderer = new Renderer(image_height, n_samples, max_depth, _camera, _scene, _image);
    connect(_renderer, SIGNAL(update_progress(double)), this, SLOT(on_progress_updated(double)));
    _renderer->start();
}

void MainWindow::on_progress_updated(double progress)
{
    ui->progressBar->setValue(100 * progress);
    ui->imageView->setPixmap(QPixmap::fromImage(_image).scaled(ui->imageView->size()));
    if (static_cast<int>(100 * progress) == 100) {
        QMessageBox::information(this, QString("提示"), QString("渲染完成"));
    }
}

