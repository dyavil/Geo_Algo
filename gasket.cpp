#include "gasket.h"

Gasket::Gasket()
{
    maillage.loadOff("/media/emeric/DATA/Documents/Etudes/M2/Geo_Algo/geo-algo/off/queen.off");
}

void Gasket::draw()
{
    maillage.drawEdges();
    //maillage.drawTriangles();
}

