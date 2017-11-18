#ifndef MAILLAGE2D
#define MAILLAGE2D

/*!
 * \file maillage2D.h
 * \brief Gestion d'un maillage 2D
 * \author Livaï Quintard - Faurobert Emeric
 */


#include <GL/gl.h>
#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <set>
#include <fstream>
#include <map>
#include "geometrie.h"
#include "myiterators.h"

// Pré-déclaration des itérateurs/circulateurs
class circulateur_de_faces;
class circulateur_de_sommets;
class marche_visibilite;


/*!
 * \class maillage2D
 * \brief Classe représentant le maillage
 *
 * Cette classe est le centre du modèle et contient
 * l'essentiel des fonctions de manipulation du
 * maillage 2D
*/
class maillage2D{
public:
    // ===== CONSTRUCTEURS & ACCESSEURS =====
    /*! \brief Constructeur
     * Constructeur par defaut
     */
    maillage2D() : sommets(0), faces(0) {}

    /*! \brief Get des sommets
     *  \return une reference sur le vecteur de sommets
     */
    std::vector<Sommet> & getSommets() { return sommets; }

    /*! \brief Get des faces
     *  \return une reference sur le vecteur de faces
     */
    std::vector<Triangle> & getFaces() { return faces; }

    /*! \brief Get des cellules de voronoï
     *  \return une reference sur le vecteur des cellules
     */
    std::vector<VoronoiCell> & getVoronoi() { return voronoiCells; }


    // ============ CIRCULATEURS ============
    /*! \brief Crer un iterateur sur les sommets
     *  \return un iterateur sur sommets.begin()
     */
    typedef std::vector<Sommet>::iterator sommet_iterator;
    sommet_iterator sommet_begin() { return sommets.begin(); }
    sommet_iterator sommet_end() { return sommets.end(); }

    /*! \brief Crer un iterateur sur les faces
     *  \return un iterateur sur faces.begin()
     */
    typedef std::vector<Triangle>::iterator face_iterator;
    face_iterator face_begin() { return faces.begin(); }
    face_iterator face_end() { return faces.end(); }

    /*! \brief Crer un circulateur sur les sommets autour d'un autre
     * \param v : sommet autour duquel est faite la circulation
     * \return le circulateur autour du sommet v
     */
    circulateur_de_sommets sommets_adjacents(Sommet & v);

    /*! \brief Crer un circulateur sur les faces autour d'un sommet
     * \param v : sommet autour duquel est faite la circulation
     * \return le circulateur de face autour du sommet v
     */
    circulateur_de_faces faces_incidente(Sommet & v);

    /*! \brief Crer un iterateur sur les faces vers un point
     * \param p : point vers lequel itere la marche
     * \return l'iterateur de marche vers le point p
     */
    marche_visibilite marche_begin(Point p);


    // ============ IMPORT/EXPORT ============
    /*! \brief Initialise un maillage vide
     */
    void initEmpty();

    /*! \brief Initialisation a partir d'un .off
     * \param filename : path du fichier à charger
     * \return true si chargement réussi, false sinon
     */
    bool loadOff(std::string filename);

    /*! \brief Initialisation a partir d'un fichier .pts, .noff, .tri
     * \param filename : path du fichier à charger
     * \param d3 : true si les point sont déclarés avec 3 dimensions
     */
    void loadPoints(std::string filename, bool d3 = true);

    //! Exporte les points vers un fichier
    /*! \brief Exporte les points vers un fichier .noff
     * \param filename : path du fichier à sauvegarder
     */
    void exportToFile(std::string filename);


    // ============ AFFICHAGE ============
    /*! \brief Affiche les triangles du maillage (avec le point infini)
     */
    void drawTriangles();

    /*! \brief Affiche les lignes du maillage (avec le point infini)
     */
    void drawEdges();

    /*! \brief Affiche les lignes du maillage (sans le point infini)
     */
    void drawEdgesTriangulation();

    /*! \brief Affiche les cercles circonscrits
     */
    void drawCircle();

    /*! \brief Affiche les cellules de Voronoï
     */
    void drawVoronoi();

    /*! \brief Affiche les lignes du maillage créer pour générer crust
     */
    void drawEdgesPreCrust();

    /*! \brief Affiche Crust
     */
    void drawCrust();


    // ============ UTILITAIRES ============
    /*! \brief Donne la position du sommet correspondant a une arête
     * \param tri : triangle concerné
     * \param i1 : sommet 1 de l'arete
     * \param i2 : sommet 2 de l'arete
     * \return l'identifiant local du sommet
     */
    int somArete(int tri, int i1, int i2);

    /*! \brief Donne les indexs de l'arête correspondante a un sommet
     * \param idt : triangle concerné
     * \param ids : sommet concerné
     * \return les identifiants de l'arete voulue
     */
    std::pair<int, int> areteSommet(int idt, int ids);

    /*! \brief Donnes les positions de l'arête commune a 2 triangles
     * \param t1 : triangle 1
     * \param t2 : triangle 2
     * \return les identifiants de l'arete commune
     */
    std::pair<int, int> somAreteCommune(int t1, int t2);

    /*! \brief Indique si les points sont dans le sens trigo
     * \param p1 : premier point du triangle
     * \param p2 : second point du triangle
     * \param p3 : troisieme point du triangle
     * \return true si le sens est trigonometrique, false sinon
     */
    bool isTrigo(Point p1, Point p2, Point p3);

    /*! \brief Indique si un point est contenu dans un triangle
     * \param p : point à tester
     * \param t : indice du triangle à tester
     * \return true si le point est contenu par t, false sinon
     */
    bool isInside(Point & p, int t);

    /*! \brief Retourne l'index du triangle contenant le point p1
     * \param p1 : point à tester
     * \return l'index de p1
     */
    int inTriangle(Point p1);

    /*! \brief Indique si un triangle a pour sommet le point infini
     * \param t : indice du triangle à tester
     * \return true si vérifié, false sinon
     */
    bool isInvisible(int t);

    /*! \brief Remet le point infini d'un triangle a la position 0 (inutilisé)
     * \param t : indice du triangle à traiter
     */
    void setInfinyAtZero(int t);

    /*! \brief Indique si 2 triangles sont "swapable" (inutilisé)
     * \param idt1 : indice du triangle à tester 1
     * \param idt2 : indice du triangle à tester 2
     * \return true si vérifié, false sinon
     */
    bool canSwap(int idt1, int idt2);

    /*! \brief Swap l'arête commune entre 2 triangles
     * \param idt1 : indice du triangle à tester 1
     * \param idt2 : indice du triangle à tester 2
     */
    void swapArete(int t1, int t2);

    /*! \brief Recupere l'arete oppose à un sommet
     * \param triangleId : indice du triangle à tester
     * \param sommetPos : indice du sommet à tester
     * \return l'arete voulue
     */
    std::pair<int, int> getSommetOppose(int trangleId, int sommetPos);


    // ============ FONCTIONS ============
    /*! \brief Construit le maillage après lecture des points
     */
    void buildMaillage();

    /*! \brief Met a jour les voisins d'apres une liste d'arete communes
     * \param faceVoisine : liste d'arete comunes
     * \param iSom : indices sommets concernés (a ajouter)
     */
    void calculVoisin(std::map<std::pair<int, int>, int> & faceVoisine, int iSom[]);

    /*! \brief Ajout d'un point (sommet) dans un triangle
     * \param idTriangle : indice du triangle dans lequel ajouter le point
     * \param p1 : indice du sommet à ajouter
     */
    void addPointIn(int idTriangle, int p1);

    /*! \brief Mise à jour des voisins après ajout de point dans un triangle
     * \param idtR : indice de l'ancien voisin
     * \param idt0 : indice du triangle avant ajout
     * \param newid : indice du sommet à ajouter
     */
    void updateNeighbors(int idtR, int idtO, int newid);

    /*! \brief Ajout d'un point (sommet) à l'exterieur du maillage
     * \param p0 : indice du sommet à ajouter
     */
    void addPointOut(int p0);

    /*! \brief Transformation du maillage en Delaunay (non incrémental)
     */
    void makeDelauney();

    /*! \brief Transformation du maillage en Delaunay (incrémental)
     * \param p0 : indice du sommet ajouté
     */
    void makeIncrementDelauney(int s);

    /*! \brief Vérification si le maillage est de Delaunay
     *  \return true si Delaunay, false sinon
     */
    bool checkDelaunay();

    /*! \brief Get du cercle circonscrit d'un triangle
     * \param idt : indice du triangle concerné
     * \return le cercle circonscrit à idt
     */
    CercleC getCenter(int idt);

    /*! \brief Get du cercle circonscrit d'un triangle
     * \param idt : pointeur sur le triangle concerné
     * \return le cercle circonscrit à idt
     */
    CercleC getCenter(Triangle *idt);

    /*! \brief Fabrication du maillage des cellules de Voronoï
     */
    void buildVoronoiCenters();

    /*! \brief Algorithme crust
     */
    void buildCrust();

    /*! \brief Ajout d'un point depuis l'interface
     *  \param np : point à ajouter
     */
    void addPointUI(Point np);

    /*! \brief Vide les buffers de sommets et faces du maillage
     */
    void clear();

private:
    // Attributs
    std::vector<Sommet> sommets;/*!< Vecteur de sommets*/
    std::vector<Triangle> faces;/*!< Vecteur de faces*/

    std::vector<Point> voronoiPoints;/*!< Vecteur temporaire de centre de Voronoï*/
    std::vector<VoronoiCell> voronoiCells;/*!< Vecteur temporaire de cellules de Voronoï*/

    int startCrust;
    std::vector<Sommet> sommetsCrust;/*!< Vecteur temporaire sommets pour le maillage crust*/
    std::vector<Sommet> sommetsPreCrust;/*!< Vecteur temporaire sommets pour le maillage precrust*/
    std::vector<Triangle> facesPreCrust;/*!< Vecteur temporaire faces pour le maillage precrust*/
};

#endif // MAILLAGE2D

