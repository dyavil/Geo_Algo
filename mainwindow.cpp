#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    color(0)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(released()), this, SLOT(onButton()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButton() {
    ++color;
    float colors[4][3] = {{1.f, 1.f, 0.f}, {1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}};
    glColor3fv(colors[color % 4]);
}
