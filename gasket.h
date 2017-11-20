#ifndef GASKET_H
#define GASKET_H

/*!
 * \file gasket.h
 * \brief lien entre l'interface et le modèle
 * \author Livaï Quintard - Faurobert Emeric
 */

#include "maillage2D.h"

/*!
 * \class Gasket
 * \brief Classe liant le modèle à la vue
*/
class Gasket
{
public:
    /*! \brief Constructeur
     * Constructeur par defaut
     */
    Gasket();

    /*! \brief Dessine les Triangles */
    void drawTriangles();
     /*! \brief Dessine les cellules de Voronoï */
    void drawVoronoi();
     /*! \brief Dessine les cercles circonscrits */
    void drawCercles();
     /*! \brief Dessine le maillage precrust */
    void drawPreCrust();
     /*! \brief Dessine le resultat de l'algorithme crust */
    void drawCrust();
     /*! \brief Charge un maillage
      * \param path : path du fichier chargé, vide si création
      */
    void rebuild(std::string path = " ");

    /*! \brief Ajout d'un point depuis l'interface
     * \param x : valeur en x du point à ajouter
     * \param y : valeur en y du point à ajouter
     */
    void addPoint(float x, float y);

    /*! \brief Sauvegarde un maillage
     * \param path : path du fichier à sauvgarder
     */
    void exportFile(std::string filename) { maillage.exportToFile(filename); }

private:
    maillage2D maillage;/*!< Maillage affiché*/
};

#endif // GASKET_H
