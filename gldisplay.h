#ifndef GLDISPLAY_H
#define GLDISPLAY_H

/*!
 * \file gldisplay.h
 * \brief interface openGL
 * \author Livaï Quintard - Faurobert Emeric
 */

#include <gasket.h>
#include <QGLWidget>
#include <QMouseEvent>
#include <QApplication>

/*!
 * \class GLDisplay
 * \brief Classe d'interaction avec openGL
*/
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
    float _zoom;/*!< Valeur de zoom sur l'affichage */
    Gasket gasket;/*!< Lien avec le modèle*/
    std::string filepath;/*!< path du fichier courant*/

    bool triangleC;/*!< Booleen d'affichage de la triangulation */
    bool voronoiC;/*!< Booleen d'affichage des cellules de Voronoï */
    bool cerclesC;/*!< Booleen d'affichage des cercles circonscrits */
    bool crust;/*!< Booleen d'affichage du resultat de crust */
    bool preCrust;/*!< Booleen d'affichage de la triangulation precrsut */

    float _angle;
    QPoint _position;/*!< Position du curseur */
};

#endif // GLDISPLAY_H
