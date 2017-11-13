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

class circulateur_de_faces;
class circulateur_de_sommets;
class marche_visibilite;

class maillage2D{

public:
    //! Constructeur par defaut
    maillage2D() : sommets(0), faces(0) {}

    //! Donne le sommet correspondant a une arête
    int somArete(int tri, int i1, int i2);

    //! Donnes les sommets des arêtes communes a 2 triangles
    std::pair<int, int> somAreteCommune(int t1, int t2);

    //! Swap l'arête commune entre 2 triangles
    void swapArete(int t1, int t2);

    //! Initialisation a partir d'un .off
    bool loadOff(std::string filename);

    //! Affiche les lignes du maillage
    void drawEdges();

    //! Affiche les lignes du maillage triangulé
    void drawEdgesTriangulation();

    //! Affiche les triangles du maillage
    void drawTriangles();

    //! Renvoie le vector de sommets
    std::vector<Sommet> & getSommets(){return sommets;}

    //! Renvoie le vector de sommets
    std::vector<Triangle> & getFaces(){return faces;}

    std::vector<VoronoiCell> & getVoronoi(){return voronoiCells;}

    //! Renvoie un circulateur de face autour du sommet v
    circulateur_de_faces faces_incidente(Sommet & v);

    //! Renvoie un circulateur de sommets autour du sommet v
    circulateur_de_sommets sommets_adjacents(Sommet & v);

    //! Iterateur sur les faces
    typedef std::vector<Triangle>::iterator face_iterator;
    face_iterator face_begin() { return faces.begin(); }
    face_iterator face_end() { return faces.end(); }

    //! Iterateur sur les sommets
    typedef std::vector<Sommet>::iterator sommet_iterator;
    sommet_iterator sommet_begin() { return sommets.begin(); }
    sommet_iterator sommet_end() { return sommets.end(); }

    marche_visibilite marche_begin(Point p);

    //! Lecture d'un fichier de points
    void loadPoints(std::string filename, bool d3 = true);

    void buildMaillage();

    void calculVoisin(std::map<std::pair<int, int>, int> & faceVoisine, int iSom[]);

    int inTriangle(Point p1);

    bool isInside(Point & p, int t);

    void addPointIn(int idTriangle, int p1);
    void updateNeighbors(int idtR, int idtO, int newid);

    void addPointOut(int p0);

    void makeDelauney();

    //a refaire
    void makeIncrementDelauney(int s);

    bool isTrigo(Point p1, Point p2, Point p3);

    // Indique si un triangle a pour sommet le point infini
    bool isInvisible(int t);

    // Remet le point infini d'un triangle a la position 0
    void setInfinyAtZero(int t);

    int getOtherSommet(int tp1, int tp2);

    std::pair<int, int> areteSommet(int idt, int ids);

    bool checkDelaunay();

    CercleC getCenter(int idt);

    CercleC getCenter(Triangle *idt);

    void drawCircle();

    void buildVoronoiCenters();

    void drawVoronoi();

    void buildCrust();

    void drawCrust();

private:
    //! Attributs
    std::vector<Sommet> sommets;
    std::vector<Triangle> faces;
    std::vector<Point> voronoiPoints;
    std::vector<VoronoiCell> voronoiCells;

    std::vector<Sommet> sommetsCrust;
    int startCrust;
    std::pair<int, int> getSommetOppose(int trangleId, int sommetPos);

    bool canSwap(int idt1, int idt2);

};

#endif // MAILLAGE2D

