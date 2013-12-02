#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "segmentedimage.h"
#include "statisticscollector.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // DEBUG
    /*SegmentedImage segImg("../Probabilistic-Color-By-Numbers/data/1198080_t.png");
    segImg.segment();*/

    StatisticsCollector sc;
    sc.collectData("../Probabilistic-Color-By-Numbers/data/");
}

MainWindow::~MainWindow()
{
    delete ui;
}
