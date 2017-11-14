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
    virtual void setShowPreCrust(bool s);
    virtual void setAngle(float a);

protected:
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );

private:
    float _zoom;
    Gasket gasket;
    std::string filepath;

    bool triangleC;
    bool voronoiC;
    bool cerclesC;
    bool crust;
    bool preCrust;

    float _angle;
    QPoint _position;
    
signals:
    
public slots:
    
};

#endif // GLDISPLAY_H
