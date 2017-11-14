#include "gldisplay.h"

#define FRUSTUM_SIZE 1.0f

GLDisplay::GLDisplay(QWidget *parent) :
    QGLWidget(parent),
    _zoom(1.0f),
    gasket(),
    triangleC(true),
    voronoiC(false),
    cerclesC(false),
    crust(false),
    preCrust(false),
    _angle(0.0f)
{
}

void GLDisplay::initializeGL()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glFrontFace(GL_CCW);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glColor3f(1.0, 1.0, 0.0);
}

void GLDisplay::zoomIn(){
    _zoom = _zoom + 0.2f;
    updateGL();
}

void GLDisplay::zoomOut(){
    _zoom = _zoom - 0.2f;
    updateGL();
}

Gasket & GLDisplay::getGasket(){
    return gasket;
}

void GLDisplay::loadMesh(QString path) {
    gasket = Gasket(path.toStdString());
    updateGL();
}

void GLDisplay::setShowTriangle(bool s){
    triangleC = s;
    updateGL();
}

void GLDisplay::setShowVoronoi(bool s){
    voronoiC = s;
    updateGL();
}

void GLDisplay::setShowCercles(bool s){
    cerclesC = s;
    updateGL();
}

void GLDisplay::setShowCrust(bool s){
    crust = s;
    updateGL();
}

void GLDisplay::setShowPreCrust(bool s){
    preCrust = s;
    updateGL();
}


void GLDisplay::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glScaled(_zoom, _zoom, _zoom);

    glRotatef(_angle, 0.0f, 1.0f, 0.0f);

    if(triangleC)gasket.drawTriangles();
    if(voronoiC) gasket.drawVoronoi();
    if(cerclesC)gasket.drawCercles();
    if(crust)gasket.drawCrust();
    if(preCrust)gasket.drawPreCrust();
    //gasket.draw();
}

void GLDisplay::setAngle(float a){
    _angle = a;
}

void GLDisplay::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);

    glViewport(0, 0, w, h);

    glOrtho(-FRUSTUM_SIZE, FRUSTUM_SIZE,
            -FRUSTUM_SIZE, FRUSTUM_SIZE,
            -FRUSTUM_SIZE, FRUSTUM_SIZE);

    glMatrixMode(GL_MODELVIEW);
}

void GLDisplay::mouseMoveEvent(QMouseEvent *event)
{
    if( event != NULL ) {
        QPoint position = event->pos();

        _angle += (position.x() - _position.x());

        _position = position;

        updateGL();
    }
}

void GLDisplay::mousePressEvent(QMouseEvent *event)
{
    if( event != NULL ){
        _position = event->pos();
        gasket.addPoint(_position.x()*((2*FRUSTUM_SIZE*(1/_zoom))/this->size().width())-FRUSTUM_SIZE*(1/_zoom), -_position.y()*((2*FRUSTUM_SIZE*(1/_zoom))/this->size().height())+FRUSTUM_SIZE*(1/_zoom));

        updateGL();
    }
}
