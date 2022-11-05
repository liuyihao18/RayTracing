#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gui_handler.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), _worker(nullptr), _ok(false)
{
    ui->setupUi(this);
    initialize();
    auto handler = GUIHandler::Inst();
    connect(handler, SIGNAL(infoMsg(QString)), this, SLOT(onInfoMsgReceived(QString)));
    connect(handler, SIGNAL(warnMsg(QString)), this, SLOT(onWarnMsgReceived(QString)));
    connect(handler, SIGNAL(errMsg(QString)), this, SLOT(onErrMsgReceived(QString)));
    connect(handler, SIGNAL(updateProgressMsg(double)), this, SLOT(onProgressUpdated(double)));
}

MainWindow::~MainWindow()
{
    if (_worker)
    {
        _worker->terminate();
        delete _worker;
    }
    delete ui;
}

void MainWindow::initialize()
{
    ui->progressBar->setValue(0);

    QStringList items;
    items << tr("120") << tr("225") << tr("360") << tr("480") << tr("640") << tr("720") << tr("1080") << tr("1600");
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
        QMessageBox::information(this, tr("提示"), tr("渲染完成"));
    }
}

void MainWindow::onInfoMsgReceived(QString infoMsg)
{
    QMessageBox::information(this, tr("提示"), infoMsg);
}

void MainWindow::onWarnMsgReceived(QString warnMsg)
{
    QMessageBox::warning(this, tr("警告"), warnMsg);
}

void MainWindow::onErrMsgReceived(QString errMsg)
{
    QMessageBox::critical(this, tr("错误"), errMsg);
    exit(1);
}

void MainWindow::on_importButton_clicked()
{
    /* World */
    // _scene = Scene::createFromJson("./scene/1.json", &_ok);
    _scene = Scene::test_scene_3();

    /* Camera */
    _camera = Camera::createFromJson("./scene/1.json", &_ok);

    /* Render */
    on_renderButton_clicked();
}

void MainWindow::on_abortButton_clicked()
{
    if (!_ok)
    {
        return;
    }

    if (_worker)
    {
        if (_worker->isFinished())
        {
            return;
        }
        _worker->terminate();
        delete _worker;
        _worker = nullptr;
    }
    else
    {
        return;
    }
    ui->progressBar->setValue(0);
    QMessageBox::information(this, tr("提示"), tr("停止成功"));
}

void MainWindow::on_renderButton_clicked()
{
    if (!_ok)
    {
        return;
    }

    if (_worker)
    {
        if (!_worker->isFinished())
        {
            return;
        }
        delete _worker;
        _worker = nullptr;
    }

    /* Image */
    int image_height = ui->imageHeight->currentText().toInt();
    int image_width = image_height * aspect_ratio;
    _image = QImage(image_width, image_height, QImage::Format_RGB888);

    /* Renderer */
    int n_samples = ui->nSamples->value();
    int max_depth = ui->maxDepth->value();
    _renderer = Renderer(image_height, n_samples, max_depth);
    auto fun = [&]
    {
        _renderer.render(_camera, _scene, _image);
    };
    _worker = QThread::create(fun);
    _worker->start();
}

void MainWindow::on_saveButton_clicked()
{
    if (!_ok)
    {
        return;
    }

    QString path = QFileDialog::getSaveFileName(this, tr("选择保存位置"), tr("./output"), tr("*.png"));
    if (!path.isEmpty())
    {
        if (_image.save(path))
        {
            QMessageBox::information(this, tr("提示"), tr("保存成功"));
        }
        else
        {
            QMessageBox::warning(this, tr("警告"), tr("保存失败"));
        }
    }
}
