#include "gasket.h"

Gasket::Gasket()
{
    maillage.loadOff("/media/emeric/DATA/Documents/Etudes/M2/Geo_Algo/geo-algo/off/testSwap.off");

    /*
    for(unsigned int i = 0; i < maillage.faces.size(); ++i) {
        std::cout << i << " | " << maillage.faces[i] << std::endl;
    }

    maillage.swapArete(0, 1);

    std::cout << "=======================" << std::endl;
    for(unsigned int i = 0; i < maillage.faces.size(); ++i) {
        std::cout << i << " | " << maillage.faces[i] << std::endl;
    }
    */

}

void Gasket::draw()
{
    maillage.drawEdges();
    //maillage.drawTriangles();
}

