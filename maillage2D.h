#ifndef MAILLAGE2D
#define MAILLAGE2D

#include <GL/gl.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include "geometrie.h"

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

    //! Affiche les triangles du maillage
    void drawTriangles();

    //! Attributs
    std::vector<Sommet> sommets;
    std::vector<Triangle> faces;

};

#endif // MAILLAGE2D

