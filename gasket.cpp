#include "gasket.h"

Gasket::Gasket()
{
    //maillage.loadOff("/home/dyavil/Documents/Master2/MaillageGeoAlgo/ToLoad/cone.off");
    maillage.loadPoints("/home/dyavil/Documents/Master2/MaillageGeoAlgo/ToLoad/points.noff");
    bool infinite_loop = false;

    //test circulators
    maillage2D::sommet_iterator iit = maillage.sommet_begin();

    int i = 0;
    while (iit != maillage.sommet_end()) {

        circulateur_de_faces circu = maillage.faces_incidente(*iit);
        int cnt=0;
        circu = circu.debut();
        do{
           ++circu;
            cnt++;
            if(cnt > 15) {
                std::cout << " break " << std::endl;
                infinite_loop =true;
                break;
            }
        }while(circu!=circu.debut());
        std::cout << i << "b, " << cnt << std::endl;
        circulateur_de_sommets circu2 = maillage.sommets_adjacents(*iit);
        cnt=0;

        circu2 = circu2.debut();
        do{
            std::cout << i << "a, " << (*circu2)->coord << std::endl;
           ++circu2;

            cnt++;
            if(cnt > 15) {
                infinite_loop =true;
                break;
            }
        }while(circu2!=circu2.debut());
        std::cout << i << "a, " << (*circu2)->coord << std::endl;
        std::cout << i << "a, " << cnt << std::endl;
        i++;
        iit++;
    }
    if(infinite_loop) std::cout << "innnnnnnnfinite looooooooooooooop" << std::endl;


    for(unsigned int i = 0; i < maillage.getFaces().size(); ++i) {
        std::cout << i << " | " << maillage.getFaces()[i] << std::endl;
    }/*

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

