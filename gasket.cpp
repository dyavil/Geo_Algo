#include "gasket.h"

Gasket::Gasket()
{
    rebuild();
}

void Gasket::rebuild(std::string path){
    maillage.clear();
    if(path == " ") maillage.initEmpty();
    else{

        std::cout << "Chargement en cours...";

        std::string ext = path.substr(path.find_last_of(".") + 1);

        if(ext == "off") {
            maillage.loadOff(path);
            maillage.drawTriangles();
            return;
        } else if(ext == "noff") {
            maillage.loadPoints(path);
        } else if(ext == "pts" || ext == "tri") {
            maillage.loadPoints(path, false);
        } else {
            std::cout << "Erreur : extension inconnue !" << std::endl;
            return;
        }

        std::cout << " terminé !" << std::endl;
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
                if(cnt > 200) {
                    std::cout << " break " << std::endl;
                    infinite_loop =true;
                    break;
                }
            }while(circu!=circu.debut());
            circulateur_de_sommets circu2 = maillage.sommets_adjacents(*iit);
            cnt=0;

            circu2 = circu2.debut();
            do{
               ++circu2;

                cnt++;
                if(cnt > 200) {
                    infinite_loop =true;
                    break;
                }
            }while(circu2!=circu2.debut());
            i++;
            iit++;
        }
        if(infinite_loop) std::cout << "innnnnnnnfinite looooooooooooooop" << std::endl;


        for(unsigned int i = 0; i < maillage.getFaces().size(); ++i) {
            //std::cout << i << " | " << maillage.getFaces()[i] << std::endl;
        }


        // ============== Marche ================
        /*Point pTest(-0.3, -0.3);
        marche_visibilite marche = maillage.marche_begin(pTest);

        for(int i = 0; i <= 20; ++i) {
            Triangle * face = *marche;

            std::cout << i << " | triangle n°" << marche.current << " = " << *face << std::endl;
            for(int i = 0; i < 3; ++i) {
                std::cout << "som : " << i << " | " << maillage.getSommets()[face->getSommets()[i]] << std::endl;
            }

            ++marche;
        }

        std::cout << "Le point (" << pTest.x << "," << pTest.y << ") est dans le triangle n°" << maillage.inTriangle(pTest) << std::endl;*/
        // ======================================


        std::cout << "Test Delaunay : " << maillage.checkDelaunay() << " (1 si vérifié)" << std::endl;
        maillage.buildVoronoiCenters();
        maillage.buildCrust();
    }
}

void Gasket::addPoint(float x, float y){
    Point np = Point(x, y);
    maillage.addPointUI(np);
}

void Gasket::drawTriangles(){
    glColor3f(1.0, 1.0, 0.0);
    maillage.drawEdgesTriangulation();
}

void Gasket::drawVoronoi(){
    glColor3f(0.0, 1.0, 0.0);
    maillage.drawVoronoi();
}

void Gasket::drawCercles(){
    glColor3f(1.0, 0.0, 0.0);
    maillage.drawCircle();
}

void Gasket::drawPreCrust(){
    maillage.drawEdgesPreCrust();
}

void Gasket::drawCrust(){
    glColor3f(1.0, 0.0, 1.0);
    maillage.drawCrust();
}
