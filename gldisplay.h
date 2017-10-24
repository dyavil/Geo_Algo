#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <QGLWidget>
#include <gasket.h>
#include <QMouseEvent>

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

    virtual void setShowTriangle(bool s);
    virtual void setShowVoronoi(bool s);
    virtual void setShowCercles(bool s);

protected:
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );

private:
    void drawSierpinski();
    void changeColor();
    Gasket gasket;

    bool triangleC;
    bool voronoiC;
    bool cerclesC;

    float _angle;
    QPoint _position;
    float _zoom;
    
signals:
    
public slots:
    
};

#endif // GLDISPLAY_H
