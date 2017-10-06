#ifndef MYITERATORS_H
#define MYITERATORS_H

#include "maillage2D.h"

class maillage2D;

class circulateur_de_faces{
public:
    circulateur_de_faces(Triangle * ft, Sommet & p, maillage2D * m):start(ft), current(ft), pivot(p), mesh(m){}
    circulateur_de_faces operator =(Triangle* s){current = s; return *this;};
    bool operator !=(Triangle* s){if(current != s) return true; return false;};
    Triangle * debut(){return start;};
    Triangle * operator ++();
    Triangle * operator *(){return current;};
    void getIdSommet();

private:

    Sommet & pivot;
    Triangle * current;
    Triangle * start;
    maillage2D * mesh;
    int indexPivot;
};

class circulateur_de_sommets{
public:
    circulateur_de_sommets(Triangle * ft, Sommet & p, maillage2D * m):currentT(ft), start(ft), current(&p), pivot(p), mesh(m), last(false){}
    circulateur_de_sommets operator =(Sommet* s);
    bool operator !=(Sommet* s){if(current != s) return true; return false;};
    Sommet * debut();
    Sommet * operator ++();
    Sommet * operator *(){return current;};
    void getIdSommet();

private:

    Sommet & pivot;
    Sommet * startP;
    Sommet * current;
    Triangle * currentT;
    Triangle * start;
    maillage2D * mesh;

    bool last;
    int indexPivot;
};



#endif // MYITERATORS_H
