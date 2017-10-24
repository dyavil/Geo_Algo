#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    int color;
    bool triangleC;
    bool voronoiC;
    bool cerclesC;

public slots:
    void onButton();
    void onTriangleCheck();
    void onVoronoiCheck();
    void onCercleCheck();
    void onZoomInButton();
    void onZoomOutButton();

};

#endif // MAINWINDOW_H
