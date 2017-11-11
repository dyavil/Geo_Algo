#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool triangleC;
    bool voronoiC;
    bool cerclesC;
    bool crust;

public slots:
    void onLoad();
    void onTriangleCheck();
    void onVoronoiCheck();
    void onCercleCheck();
    void onCrustCheck();
    void onZoomInButton();
    void onZoomOutButton();

};

#endif // MAINWINDOW_H
