#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    triangleC(true),
    voronoiC(false),
    cerclesC(false),
    crust(false)
{
    ui->setupUi(this);
    ui->triangleCheck->setChecked(true);
    connect(ui->loadButton, SIGNAL(released()), this, SLOT(onLoad()));
    connect(ui->saveButton, SIGNAL(released()), this, SLOT(onWrite()));
    connect(ui->restartButton, SIGNAL(released()), this, SLOT(onRestart()));
    connect(ui->triangleCheck, SIGNAL(clicked(bool)), this, SLOT(onTriangleCheck()));
    connect(ui->voronoiCheck, SIGNAL(clicked(bool)), this, SLOT(onVoronoiCheck()));
    connect(ui->circonCheck, SIGNAL(clicked(bool)), this, SLOT(onCercleCheck()));
    connect(ui->crustBox, SIGNAL(clicked(bool)), this, SLOT(onCrustCheck()));
    connect(ui->zoomInButton, SIGNAL(released()), this, SLOT(onZoomInButton()));
    connect(ui->zoomOutButton, SIGNAL(released()), this, SLOT(onZoomOutButton()));
    ui->widget->setShowTriangle(triangleC);
    ui->widget->setShowVoronoi(voronoiC);
    ui->widget->setShowCercles(cerclesC);
    ui->widget->setShowCrust(crust);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTriangleCheck(){
    if(triangleC) triangleC = false;
    else triangleC = true;
    ui->widget->setShowTriangle(triangleC);

}

void MainWindow::onVoronoiCheck(){
    if(voronoiC) voronoiC = false;
    else voronoiC = true;
    ui->widget->setShowVoronoi(voronoiC);

}

void MainWindow::onCercleCheck(){
    if(cerclesC) cerclesC = false;
    else cerclesC = true;
    ui->widget->setShowCercles(cerclesC);
}

void MainWindow::onCrustCheck(){
    if(crust) crust = false;
    else crust = true;
    ui->widget->setShowCrust(crust);
}


void MainWindow::onZoomInButton(){
    ui->widget->zoomIn();
}

void MainWindow::onZoomOutButton(){
    ui->widget->zoomOut();
}

void MainWindow::onLoad() {
    QString filepath = QFileDialog::getOpenFileName(this, "Charger un fichier...", "../geo-algo/off/");
    ui->widget->loadMesh(filepath);
}

void MainWindow::onWrite(){
    QString filepath = QFileDialog::getSaveFileName(this, "Enregistrer un fichier...", "../geo-algo/off/");
    ui->widget->getGasket().exportFile(filepath.toStdString());
}

void MainWindow::onRestart(){
    ui->widget->getGasket() = Gasket();
    ui->widget->updateGL();
}
