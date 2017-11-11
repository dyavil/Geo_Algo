#include "gasket.h"

Gasket::Gasket()
{
    // /media/emeric/DATA/Documents/Etudes/M2/Geo_Algo/geo-algo/off/
    // /home/dyavil/Documents/Master2/MaillageGeoAlgo/ToLoad

    //maillage.loadOff("/home/dyavil/Documents/Master2/MaillageGeoAlgo/ToLoad/cone.off");
    //maillage.loadPoints("/home/dyavil/Documents/Master2/MaillageGeoAlgo/geo-algo/off/livai.noff");
    maillage.loadPoints("/media/emeric/DATA/Documents/Etudes/M2/Geo_Algo/geo-algo/off/points1.pts", false);
    bool infinite_loop = false;
    maillage.makeDelauney();
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
            if(cnt > 20) {
                std::cout << " break " << std::endl;
                infinite_loop =true;
                break;
            }
        }while(circu!=circu.debut());
        //std::cout << i << "b, " << cnt << std::endl;
        circulateur_de_sommets circu2 = maillage.sommets_adjacents(*iit);
        cnt=0;

        circu2 = circu2.debut();
        do{
           //std::cout << i << "a, " << (*circu2)->coord << std::endl;
           ++circu2;

            cnt++;
            if(cnt > 20) {
                infinite_loop =true;
                break;
            }
        }while(circu2!=circu2.debut());
        //std::cout << i << "a, " << (*circu2)->coord << std::endl;
        //std::cout << i << "a, " << cnt << std::endl;
        i++;
        iit++;
    }
    if(infinite_loop) std::cout << "innnnnnnnfinite looooooooooooooop" << std::endl;


    for(unsigned int i = 0; i < maillage.getFaces().size(); ++i) {
        //std::cout << i << " | " << maillage.getFaces()[i] << std::endl;
    }


    // ============== Marche ================

    Point pTest(-0.3, -0.3);

    marche_visibilite marche = maillage.marche_begin(pTest);

    for(int i = 0; i <= 20; ++i) {
        Triangle * face = *marche;

        std::cout << i << " | triangle n°" << marche.current << " = " << *face << std::endl;
        for(int i = 0; i < 3; ++i) {
            std::cout << "som : " << i << " | " << maillage.getSommets()[face->getSommets()[i]] << std::endl;
        }

        ++marche;
    }

    std::cout << "Le point (" << pTest.x << "," << pTest.y << ") est dans le triangle n°" << maillage.inTriangle(pTest) << std::endl;

    // ======================================


    //maillage.makeIncrementDelauney(1);

    //std::cout << "test del : " << maillage.checkDelaunay() << std::endl;
    maillage.buildVoronoiCenters();
    maillage.buildCrust();
    /*int ss[3] = {maillage.getFaces()[2].getSommets()[0], maillage.getFaces()[2].getSommets()[1], maillage.getFaces()[2].getSommets()[2]};
    Delaunay d;
    bool res = d.isOutCircle(maillage.getSommets()[ss[0]].getPoint(), maillage.getSommets()[ss[1]].getPoint(), maillage.getSommets()[ss[2]].getPoint(), Point(0, -4, 0));
    std::cout << "res : " << res << maillage.getFaces()[2] << std::endl;*/

    //si res, en dehors du cercle
    /*
    maillage.swapArete(0, 1);

    std::cout << "=======================" << std::endl;
    for(unsigned int i = 0; i < maillage.faces.size(); ++i) {
        std::cout << i << " | " << maillage.faces[i] << std::endl;
    }
    */
}

void Gasket::draw()
{
    /*glColor3f(1.0, 1.0, 0.0);
    maillage.drawEdgesTriangulation();
    glColor3f(1.0, 0.0, 0.0);
    maillage.drawCircle();
    glColor3f(0.0, 1.0, 0.0);
    maillage.drawVoronoi();*/
    //maillage.drawEdges();
}

void Gasket::drawTriangles(){
    glColor3f(1.0, 1.0, 0.0);
    maillage.drawEdgesTriangulation();
   // maillage.drawEdges();
}

void Gasket::drawVoronoi(){
    glColor3f(0.0, 1.0, 0.0);
    maillage.drawVoronoi();
}

void Gasket::drawCercles(){
    glColor3f(1.0, 0.0, 0.0);
    maillage.drawCircle();
}

void Gasket::drawCrust(){
    glColor3f(1.0, 0.0, 1.0);
    maillage.drawCrust();
}
