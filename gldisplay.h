#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <gasket.h>
#include <QGLWidget>
#include <QMouseEvent>
#include <QApplication>

class GLDisplay : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLDisplay(QWidget *parent = 0);

    virtual void initializeGL();

    virtual void paintGL();

    virtual void resizeGL(int w, int h);

    virtual void zoomIn();

    virtual void zoomOut();

    virtual Gasket & getGasket();

    virtual void loadMesh(QString path);
    virtual void setShowTriangle(bool s);
    virtual void setShowVoronoi(bool s);
    virtual void setShowCercles(bool s);
    virtual void setShowCrust(bool s);

protected:
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );

private:
    void drawSierpinski();
    void changeColor();
    Gasket gasket;
    std::string filepath;

    bool triangleC;
    bool voronoiC;
    bool cerclesC;
    bool crust;

    float _angle;
    QPoint _position;
    float _zoom;
    
signals:
    
public slots:
    
};

#endif // GLDISPLAY_H
