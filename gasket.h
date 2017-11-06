#ifndef GASKET_H
#define GASKET_H

#include "maillage2D.h"



class Gasket
{
public:
    Gasket();

    void draw();

    void drawTriangles();

    void drawVoronoi();

    void drawCercles();

    void drawCrust();

private:
    maillage2D maillage;
};

#endif // GASKET_H
