#ifndef MAINWINDOW_H
#define MAINWINDOW_H


/*!
 * \file mainwindow.h
 * \brief interface graphique globale
 * \author Livaï Quintard - Faurobert Emeric
 */

#include <QMainWindow>
#include <QFileDialog>
namespace Ui {
class MainWindow;
}

/*!
 * \class MainWindow
 * \brief Classe d'interface graphique
*/
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
    bool preCrust;

public slots:
    /*! \brief Chargement Fichier */
    void onLoad();
    /*! \brief Ecriture Fichier */
    void onWrite();
    /*! \brief Nouveau maillage */
    void onRestart();
    /*! \brief Affichage triangulation */
    void onTriangleCheck();
    /*! \brief Affichage cellules de voronoï */
    void onVoronoiCheck();
    /*! \brief Affichage cercles circonscrits */
    void onCercleCheck();
    /*! \brief Affichage crust */
    void onCrustCheck();
    /*! \brief Affichage triangulation precrust */
    void onPreCrustCheck();

    /*! \brief Changement du zoom(+) */
    void onZoomInButton();
    /*! \brief Changement du zoom(-) */
    void onZoomOutButton();
};

#endif // MAINWINDOW_H
