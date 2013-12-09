#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sstream>
#include <iostream>

#include "segmentedimage.h"
#include "statisticscollector.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // DEBUG
    std::vector<std::pair<double, SegmentedImage> > topImgs;

    SegmentedImage segImg("../Probabilistic-Color-By-Numbers/data/1198080_t.png");//1537398_t.png");
    segImg.segment();

    StatisticsCollector sc;
    sc.collectData("../Probabilistic-Color-By-Numbers/data/");
    topImgs = sc.sample(10, segImg);

    for (int i=0, n=topImgs.size(); i<n; i++) {

        std::ostringstream ss;
        ss << "../Probabilistic-Color-By-Numbers/data/suggestions/random_" << i << '_' << topImgs[i].first << ".png";
        std::cout << ss.str().c_str() << std::endl;

        topImgs[i].second.save(QString(ss.str().c_str()));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
