#ifndef GASKET_H
#define GASKET_H

#include "maillage2D.h"

class Gasket
{
public:
    Gasket() {maillage.initEmpty();}
    Gasket(std::string path);

    void draw();
    void drawTriangles();
    void drawVoronoi();
    void drawCercles();
    void drawCrust();

    void addPoint(float x, float y);
    void exportFile(std::string filename){maillage.exportToFile(filename);}

private:
    maillage2D maillage;
    //void updateRapport()
};

#endif // GASKET_H
