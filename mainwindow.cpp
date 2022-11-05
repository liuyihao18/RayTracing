#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "scene.h"
#include "camera.h"
#include "renderer.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), _renderer(nullptr)
{
    ui->setupUi(this);
    initialize();
}

MainWindow::~MainWindow()
{
    if (_renderer)
    {
        _renderer->terminate();
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

void MainWindow::onProgressUpdated(double progress)
{
    ui->progressBar->setValue(100 * progress);
    ui->imageView->setPixmap(QPixmap::fromImage(_image).scaled(ui->imageView->size()));
    if (static_cast<int>(100 * progress) == 100)
    {
        QMessageBox::information(this, QString("提示"), QString("渲染完成"));
    }
}

void MainWindow::onErrorMsgReceived(QString errMsg)
{
    QMessageBox::critical(this, "Error", errMsg);
    exit(1);
}

void MainWindow::on_importButton_clicked()
{
    /* World */
    _scene = Scene::final_scene();

    /* Camera */
    Point eye(-2, 1.11, 2), lookat(0, 0.88, 0), up(0, 1, 0);
    auto vfov = 60.0;
    _camera = Camera(eye, lookat, up, vfov);

    /* Render */
    on_renderButton_clicked();
}

void MainWindow::on_abortButton_clicked()
{
    if (_renderer)
    {
        if (_renderer->isFinished())
        {
            return;
        }
        _renderer->terminate();
        delete _renderer;
        _renderer = nullptr;
    }
    else
    {
        return;
    }
    ui->progressBar->setValue(0);
    QMessageBox::information(this, QString("提示"), QString("停止成功"));
}

void MainWindow::on_renderButton_clicked()
{
    if (_renderer)
    {
        if (!_renderer->isFinished())
        {
            return;
        }
        delete _renderer;
        _renderer = nullptr;
    }

    /* Image */
    int image_height = ui->imageHeight->currentText().toInt();
    int image_width = image_height * aspect_ratio;
    _image = QImage(image_width, image_height, QImage::Format_RGB888);

    /* Renderer */
    int n_samples = ui->nSamples->value();
    int max_depth = ui->maxDepth->value();
    _renderer = new Renderer(_camera, _scene, _image, image_height, n_samples, max_depth);
    connect(_renderer, SIGNAL(update_progress(double)), this, SLOT(onProgressUpdated(double)));
    _renderer->start();
}

void MainWindow::on_saveButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("选择保存位置"), tr("./output"), tr("*.png"));
    if (!path.isEmpty())
    {
        if (_image.save(path))
        {
            QMessageBox::information(this, QString("提示"), QString("保存成功"));
        }
        else
        {
            QMessageBox::warning(this, QString("警告"), QString("保存失败"));
        }
    }
}
