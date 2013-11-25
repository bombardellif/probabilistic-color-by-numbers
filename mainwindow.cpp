#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "segmentedimage.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // DEBUG
    SegmentedImage segImg("../Probabilistic-Color-By-Numbers/data/1198080_t.png");
    segImg.segment();
}

MainWindow::~MainWindow()
{
    delete ui;
}
