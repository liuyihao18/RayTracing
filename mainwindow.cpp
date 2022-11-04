#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize()
{
    ui->progressBar->setValue(0);

    QStringList items;
    items << QString("225") << QString("360") << QString("480") << QString("640") << QString("720") << QString("1080") << QString("1600");
    ui->imageWidth->addItems(items);

    ui->nSamples->setMinimum(1);
    ui->nSamples->setMaximum(10000);
    ui->nSamples->setValue(100);

    ui->maxDepth->setMinimum(1);
    ui->maxDepth->setMaximum(100);
    ui->maxDepth->setValue(50);
}

