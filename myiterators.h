#ifndef MYITERATORS_H
#define MYITERATORS_H

/*!
 * \file myiterators.h
 * \brief iterateurs et circulateurs
 * \author Livaï Quintard - Faurobert Emeric
 */

#include "maillage2D.h"

class maillage2D;

/*!
 * \class circulateur_de_sommets
 * \brief Classe de circulation autour d'un sommet
*/
class circulateur_de_sommets{
public:
    circulateur_de_sommets(Triangle * ft, Sommet & p, maillage2D * m):currentT(ft), start(ft), current(&p), pivot(p), mesh(m), last(false){}
    circulateur_de_sommets operator =(Sommet* s) { current = s; return *this; }
    bool operator !=(Sommet* s) { if(current != s) return true; return false; }
    Sommet * debut() { return startP; }
    Sommet * operator ++();
    Sommet * operator *() { return current; }
    void getIdSommet();

private:
    Triangle * currentT;
    Triangle * start;
    Sommet * current;
    Sommet & pivot;
    maillage2D * mesh;
    bool last;
    Sommet * startP;
    int indexPivot;
};

/*!
 * \class circulateur_de_faces
 * \brief Classe de circulation autour d'un sommet
*/
class circulateur_de_faces{
public:
    circulateur_de_faces(Triangle * ft, Sommet & p, maillage2D * m):start(ft), current(ft), pivot(p), mesh(m){}
    circulateur_de_faces operator =(Triangle* s) { current = s; return *this; }
    bool operator !=(Triangle* s) { if(current != s) return true; return false; }
    Triangle * debut() { return start; }
    Triangle * operator ++();
    Triangle * operator *() { return current; }
    void getIdSommet();

private: 
    Triangle * start;
    Triangle * current;
    Sommet & pivot;
    maillage2D * mesh;
    int indexPivot;
};

/*!
 * \class marche_visibilite
 * \brief Classe de marche vers un triangle
*/
class marche_visibilite{
public:
    marche_visibilite(Point & p, maillage2D * m) : point(p), current(0), mesh(m), index(0) {}
    Triangle * operator ++();
    Triangle * operator *();

    Point point;
    int current;
    maillage2D * mesh;
    int index;
};

#endif // MYITERATORS_H
