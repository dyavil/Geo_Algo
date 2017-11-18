#ifndef MAILLAGE2D
#define MAILLAGE2D

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

class maillage2D{
public:
    // ===== CONSTRUCTEURS & ACCESSEURS =====
    //! Constructeur par defaut
    maillage2D() : sommets(0), faces(0) {}

    //! Renvoie le vector de sommets
    std::vector<Sommet> & getSommets() { return sommets; }

    //! Renvoie le vector de faces
    std::vector<Triangle> & getFaces() { return faces; }

    //! Renvoie le vector de cellules de Voronoi
    std::vector<VoronoiCell> & getVoronoi() { return voronoiCells; }


    // ============ CIRCULATEURS ============
    //! Iterateur sur les sommets
    typedef std::vector<Sommet>::iterator sommet_iterator;
    sommet_iterator sommet_begin() { return sommets.begin(); }
    sommet_iterator sommet_end() { return sommets.end(); }

    //! Iterateur sur les faces
    typedef std::vector<Triangle>::iterator face_iterator;
    face_iterator face_begin() { return faces.begin(); }
    face_iterator face_end() { return faces.end(); }

    //! Renvoie un circulateur de sommets autour du sommet v
    circulateur_de_sommets sommets_adjacents(Sommet & v);

    //! Renvoie un circulateur de face autour du sommet v
    circulateur_de_faces faces_incidente(Sommet & v);

    //! Renvoie un itetrateur de face se dirigeant vers le point p
    marche_visibilite marche_begin(Point p);


    // ============ IMPORT/EXPORT ============
    //! Initialisation par défaut du maillage
    void initEmpty();

    //! Initialisation a partir d'un .off
    bool loadOff(std::string filename);

    //! Initialisation a partir d'un fichier .pts
    void loadPoints(std::string filename, bool d3 = true);

    //!
    void calculVoisin(std::map<std::pair<int, int>, int> & faceVoisine, int iSom[]);

    //! Construit le maillage à partir des points
    void buildMaillage();

    //! Exporte les points vers un fichier
    void exportToFile(std::string filename);


    // ============ AFFICHAGE ============
    //! Affiche les triangles du maillage (avec le point infini)
    void drawTriangles();

    //! Affiche les lignes du maillage (avec le point infini)
    void drawEdges();

    //! Affiche les lignes du maillage (sans le point infini)
    void drawEdgesTriangulation();

    //! Affiche les cercles circonscrits
    void drawCircle();

    //! Affiche les cellules de Voronoï
    void drawVoronoi();

    //!
    void drawEdgesPreCrust();

    //! Affiche Crust
    void drawCrust();


    // ============ UTILITAIRES ============
    //! Donne la position du sommet correspondant a une arête
    int somArete(int tri, int i1, int i2);

    //! Donne les indexs de l'arête correspondante a un sommet
    std::pair<int, int> areteSommet(int idt, int ids);

    //! Donnes les positions de l'arête commune a 2 triangles
    std::pair<int, int> somAreteCommune(int t1, int t2);

    //! Donne l'index du sommet opposé de la position p dans le triangle tId
    std::pair<int, int> getSommetOppose(int trangleId, int sommetPos);

    //! Indique si un triangle a pour sommet le point infini
    bool isInvisible(int t);

    //! Indique si les points sont dans le sens trigo
    bool isTrigo(Point p1, Point p2, Point p3);

    //! Indique si le point p est contenu dans le triangle t
    bool isInside(Point & p, int t);

    //! Retourne l'index du triangle contenant le point p1
    int inTriangle(Point p1);

    //! Remet le point infini d'un triangle a la position 0 (inutilisé)
    void setInfinyAtZero(int t);

    //!
    void updateNeighbors(int idtR, int idtO, int newid);

    //! Ajoute un point a l'intérieur du maillage
    void addPointIn(int idTriangle, int p1);

    //! Ajoute un point a l'extérieur du maillage
    void addPointOut(int p0);

    //! Ajoute un point au maillage
    void addPointUI(Point np);

    //! Indique si 2 triangles sont "swapable" (inutilisé)
    bool canSwap(int idt1, int idt2);

    //! Swap l'arête commune entre 2 triangles
    void swapArete(int t1, int t2);


    // ============ FONCTIONS ============

    //!
    void makeDelauney();

    //!
    void makeIncrementDelauney(int s);

    //! Indique si le maillage est globalement de Delaunay
    bool checkDelaunay();

    //!
    void buildVoronoiCenters();

    //!
    void buildCrust();


    CercleC getCenter(int idt);

    CercleC getCenter(Triangle *idt);


private:
    // Attributs
    std::vector<Sommet> sommets;
    std::vector<Triangle> faces;

    std::vector<Point> voronoiPoints;
    std::vector<VoronoiCell> voronoiCells;

    int startCrust;
    std::vector<Sommet> sommetsCrust;
    std::vector<Sommet> sommetsPreCrust;
    std::vector<Triangle> facesPreCrust;    
};

#endif // MAILLAGE2D

