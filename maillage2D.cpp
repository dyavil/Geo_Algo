#include "maillage2D.h"

//créer un circulateur de face autour de v
circulateur_de_faces maillage2D::faces_incidente(Sommet & v){
    circulateur_de_faces r = circulateur_de_faces(&faces[v.face], v, this);
    r.getIdSommet();
    return r;
}

circulateur_de_sommets maillage2D::sommets_adjacents(Sommet & v){
    circulateur_de_sommets r = circulateur_de_sommets(&faces[v.face], v, this);
    r.getIdSommet();
    return r;
}



// Donne le sommet correspondant a une arête
int maillage2D::somArete(int tri, int i1, int i2) {
    for(int i = 0; i < 3; i++) {
       if(faces[tri].getSommets()[i] == i1 && faces[tri].getSommets()[(i+1)%3] == i2) {
           return (i+2)%3;
       }
    }
    return -1;
}


// Donnes les sommets des arêtes communes a 2 faces
std::pair<int, int> maillage2D::somAreteCommune(int t1, int t2) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(faces[t1].getSommets()[i] == faces[t2].getSommets()[(j+1)%3] && faces[t1].getSommets()[(i+1)%3] == faces[t2].getSommets()[j]) {
                return std::pair<int, int> ((i+2)%3, (j+2)%3);
            }
        }
    }
    return std::pair<int, int> (-1, -1);
}


// Swap l'arête commune entre 2 faces
void maillage2D::swapArete(int t1, int t2) {
    Triangle copyT1 = faces[t1];
    Triangle copyT2 = faces[t2];

    std::pair<int, int> aretesCommunes = somAreteCommune(t1, t2);
    int ac1 = aretesCommunes.first;
    int ac2 = aretesCommunes.second;

    // Mise a jour des faces
    faces[t1].getSommets()[(ac1+2)%3] = copyT2.getSommets()[ac2];
    faces[t2].getSommets()[(ac2+2)%3] = copyT1.getSommets()[ac1];

    // Mise a jour des voisins
    faces[t1].getVoisins()[ac1] = copyT2.getVoisins()[(ac2+1)%3];
    faces[t1].getVoisins()[(ac1+1)%3] = t2;
    faces[t1].getVoisins()[(ac1+2)%3] == copyT1.getVoisins()[(ac1+2)%3];
    faces[t2].getVoisins()[ac2] = copyT1.getVoisins()[(ac1+1)%3];
    faces[t2].getVoisins()[(ac2+1)%3] = t1;
    faces[t2].getVoisins()[(ac2+2)%3] = copyT2.getVoisins()[(ac2+2)%3];

    // Mise a jour des voisins des voisins
    faces[copyT2.getVoisins()[(ac2+1)%3]].getVoisins()[somArete(copyT2.getVoisins()[(ac2+1)%3], copyT2.getSommets()[ac2], copyT2.getSommets()[(ac2+2)%3])] = t1;
    faces[copyT1.getVoisins()[(ac1+1)%3]].getVoisins()[somArete(copyT1.getVoisins()[(ac1+1)%3], copyT1.getSommets()[ac1], copyT1.getSommets()[(ac1+2)%3])] = t2;
}


// Initialisation a partir d'un .off
bool maillage2D::loadOff(std::string filename) {
    std::ifstream file(filename.c_str());
    std::string format;
    int nbSommets, nbFaces, nbEdges;
    Point point;
    int nbSomFace, iSom[3];
    std::map<std::pair<int, int>, int> faceVoisine;
    std::map<std::pair<int, int>, int>::iterator it;

    // Vérification du format
    file >> format;
    if(format != "OFF"){
        std::cout << "Not a .off file" << std::endl;
        return false;
    }

    // On resize nos vecotrs
    file >> nbSommets >> nbFaces >> nbEdges;
    sommets.resize(nbSommets);
    faces.resize(nbFaces);

    // Initialisation des sommets
    for(int i = 0; i < nbSommets; i++) {
        file >> point.x >> point.y >> point.z;
        sommets[i] = Sommet(point);
    }

    // Initialisation des faces
    for(int i = 0; i < nbFaces; i++) {
        file >> nbSomFace >> iSom[0] >> iSom[1] >> iSom[2];
        faces[i] = Triangle(iSom[0], iSom[1], iSom[2]);

        // Face voisine du sommet
        for(int j = 0; j < 3; j++) {
            if(sommets[iSom[j]].face == -1) {
                sommets[iSom[j]].face = i;
            }
        }

        // Faces voisines de la face
        it = faceVoisine.find(std::make_pair(iSom[0], iSom[1]));
        if(it != faceVoisine.end()) {
            faces[i].getVoisins()[2] = it->second;
            faces[it->second].getVoisins()[somArete(it->second, iSom[1], iSom[0])] = i;
        } else {
            faceVoisine.insert(std::pair<std::pair<int, int>, int> (std::make_pair(iSom[1], iSom[0]), i));
        }

        it = faceVoisine.find(std::make_pair(iSom[1], iSom[2]));
        if(it != faceVoisine.end()) {
            faces[i].getVoisins()[0] = it->second;
            faces[it->second].getVoisins()[somArete(it->second, iSom[2], iSom[1])] = i;
        } else {
            faceVoisine.insert(std::pair<std::pair<int, int>, int> (std::make_pair(iSom[2], iSom[1]), i));
        }

        it = faceVoisine.find(std::make_pair(iSom[2], iSom[0]));
        if(it != faceVoisine.end()) {
            faces[i].getVoisins()[1] = it->second;
            faces[it->second].getVoisins()[somArete(it->second, iSom[0], iSom[2])] = i;
        } else {
            faceVoisine.insert(std::pair<std::pair<int, int>, int> (std::make_pair(iSom[0], iSom[2]), i));
        }
    }

    return true;
}


// Affiche les lignes du maillage
void maillage2D::drawEdges() {
    for(unsigned int i = 0; i < faces.size(); i++) {
        glBegin(GL_LINE_LOOP);
        glVertex3f(sommets[faces[i].getSommets()[0]].coord.x, sommets[faces[i].getSommets()[0]].coord.y, sommets[faces[i].getSommets()[0]].coord.z);
        glVertex3f(sommets[faces[i].getSommets()[1]].coord.x, sommets[faces[i].getSommets()[1]].coord.y, sommets[faces[i].getSommets()[1]].coord.z);
        glVertex3f(sommets[faces[i].getSommets()[2]].coord.x, sommets[faces[i].getSommets()[2]].coord.y, sommets[faces[i].getSommets()[2]].coord.z);
        glEnd();
    }
}


// Affiche les lignes du maillage triangulé
void maillage2D::drawEdgesTriangulation() {
    for(unsigned int i = 0; i < faces.size(); i++) {
        glBegin(GL_LINE_LOOP);
        if(faces[i].getSommets()[0] != 0 && faces[i].getSommets()[1] != 0 && faces[i].getSommets()[2] != 0){
            glVertex3f(sommets[faces[i].getSommets()[0]].coord.x, sommets[faces[i].getSommets()[0]].coord.y, sommets[faces[i].getSommets()[0]].coord.z);
            glVertex3f(sommets[faces[i].getSommets()[1]].coord.x, sommets[faces[i].getSommets()[1]].coord.y, sommets[faces[i].getSommets()[1]].coord.z);
            glVertex3f(sommets[faces[i].getSommets()[2]].coord.x, sommets[faces[i].getSommets()[2]].coord.y, sommets[faces[i].getSommets()[2]].coord.z);
        }
        glEnd();
    }
}


// Affiche les faces du maillage
void maillage2D::drawTriangles() {
    glBegin(GL_TRIANGLES);
    for(unsigned int i = 0; i < faces.size(); i++) {
        glVertex3f(sommets[faces[i].getSommets()[0]].coord.x, sommets[faces[i].getSommets()[0]].coord.y, sommets[faces[i].getSommets()[0]].coord.z);
        glVertex3f(sommets[faces[i].getSommets()[1]].coord.x, sommets[faces[i].getSommets()[1]].coord.y, sommets[faces[i].getSommets()[1]].coord.z);
        glVertex3f(sommets[faces[i].getSommets()[2]].coord.x, sommets[faces[i].getSommets()[2]].coord.y, sommets[faces[i].getSommets()[2]].coord.z);
    }
    glEnd();
}


void maillage2D::loadPoints(std::__cxx11::string filename){

    std::ifstream file;
    Sommet infinite = Sommet(Point(0, 0, -1), -1);
    sommets.push_back(infinite);
    file.open(filename);
    unsigned int nbs;
    file >> nbs;
    float x0, y0, z0;
    for(unsigned int i = 0; i < nbs; i++){
        file >> x0 >> y0 >> z0;
        sommets.push_back(Sommet(Point(x0, y0, z0), -1));
    }
    file.close();
    buildMaillage();
}




void maillage2D::buildMaillage(){

    bool first = true;
    std::map<std::pair<int, int>, int> corresp;
    for (int i = 1; i < sommets.size(); ++i) {
        //premier triangle
        if(first){
            if(sommets.size() < 4 ) return;
            Vector3 vec1 = Vector3(sommets[i].getPoint(), sommets[i+1].getPoint());
            Vector3 vec2 = Vector3(sommets[i].getPoint(), sommets[i+2].getPoint());
            Vector3 dir;
            dir = dir.cross(vec1, vec2);

            if(dir.z > 0){
                faces.push_back(Triangle(i, i+1, i+2, 3, 1, 2));
            }else faces.push_back(Triangle(i, i+2, i+1, 3, 2, 1));

            if(sommets[i].getFace() == -1) sommets[i].setFace(0);
            if(sommets[i+1].getFace() == -1) sommets[i+1].setFace(0);
            if(sommets[i+2].getFace() == -1) sommets[i+2].setFace(0);
            if(sommets[0].getFace() == -1) sommets[0].setFace(faces.size());

            ///////test/////
            //0 1 3
            int iSom[] = {0, i, i+2};
            if(dir.z > 0) faces.push_back(Triangle(0, i, i+2, 0, -1, -1));
            else {
                faces.push_back(Triangle(0, i+2, i, 0, -1, -1));
                iSom[1] = i+2;
                iSom[2] = i;
            }

            calculVoisin(corresp, iSom);

            // 0 2 1
            int iSom2[] = {0, i+1, i};
            if(dir.z > 0) faces.push_back(Triangle(0, i+1, i, 0, -1, -1));
            else {
                faces.push_back(Triangle(0, i, i+1, 0, -1, -1));
                iSom2[1] = i;
                iSom2[2] = i+1;
            }

            calculVoisin(corresp, iSom2);


            //0 3 2
            int iSom3[] = {0, i+2, i+1};
            if(dir.z > 0) faces.push_back(Triangle(0, i+2, i+1, 0, -1, -1));
            else {
                faces.push_back(Triangle(0, i+1, i+2, 0, -1, -1));
                iSom3[1] = i+1;
                iSom3[2] = i+2;
            }



            calculVoisin(corresp, iSom3);


            //connexion au point infini
            i += 2;
            first=false;
        }else{
            //ajout du reste des points

            unsigned int pos = inTriangle(sommets[i].getPoint());
            if(pos == (unsigned int)-1) {
                addPointOut(i);
            }
            else {
                addPointIn(pos, i);
            }
        }

    }
}


void maillage2D::calculVoisin(std::map<std::pair<int, int>, int> & faceVoisine, int iSom[]){
    std::map<std::pair<int, int>, int>::iterator it;
    int i = faces.size()-1;
    // Faces voisines de la face
    it = faceVoisine.find(std::make_pair(iSom[0], iSom[1]));
    if(it != faceVoisine.end()) {
        faces[i].getVoisins()[2] = it->second;
        faces[it->second].getVoisins()[somArete(it->second, iSom[1], iSom[0])] = i;
    } else {
        faceVoisine.insert(std::pair<std::pair<int, int>, int> (std::make_pair(iSom[1], iSom[0]), i));
    }

    it = faceVoisine.find(std::make_pair(iSom[1], iSom[2]));
    if(it != faceVoisine.end()) {
        faces[i].getVoisins()[0] = it->second;
        faces[it->second].getVoisins()[somArete(it->second, iSom[2], iSom[1])] = i;
    } else {
        faceVoisine.insert(std::pair<std::pair<int, int>, int> (std::make_pair(iSom[2], iSom[1]), i));
    }

    it = faceVoisine.find(std::make_pair(iSom[2], iSom[0]));
    if(it != faceVoisine.end()) {
        faces[i].getVoisins()[1] = it->second;
        faces[it->second].getVoisins()[somArete(it->second, iSom[0], iSom[2])] = i;
    } else {
        faceVoisine.insert(std::pair<std::pair<int, int>, int> (std::make_pair(iSom[0], iSom[2]), i));
    }

}





int maillage2D::inTriangle(Point p1){
    int triangleId = -1;
    for(unsigned int i = 0; i < faces.size(); ++i){
        bool cont = true;
        if(faces[i].getSommets()[0] == 0 || faces[i].getSommets()[1] == 0 || faces[i].getSommets()[2] == 0) cont = false;

        if (cont){
        Vector3 vec1 = Vector3(p1, sommets[faces[i].getSommets()[0]].getPoint());
        Vector3 vec2 = Vector3(p1, sommets[faces[i].getSommets()[1]].getPoint());
        Vector3 dir;
        dir = dir.cross(vec1, vec2);
        if(dir.z < 0) cont = false;
            if(cont){
                vec1 = Vector3(p1, sommets[faces[i].getSommets()[1]].getPoint());
                vec2 = Vector3(p1, sommets[faces[i].getSommets()[2]].getPoint());
                dir = dir.cross(vec1, vec2);
                if(dir.z < 0) cont = false;
                if(cont){
                    vec1 = Vector3(p1, sommets[faces[i].getSommets()[2]].getPoint());
                    vec2 = Vector3(p1, sommets[faces[i].getSommets()[0]].getPoint());
                    dir = dir.cross(vec1, vec2);
                    if(dir.z < 0) cont = false;
                    else {
                        triangleId = i;
                        break;
                    }
                }
            }
        }
    }
    return triangleId;
}


void maillage2D::addPointIn(int idTriangle, int p1){
    int oldp1 = faces[idTriangle].getSommets()[1];
    faces[idTriangle].getSommets()[1] = p1;

    int oldt0 = faces[idTriangle].getVoisins()[0];
    faces[idTriangle].getVoisins()[0] = -1;
    int oldt2 = faces[idTriangle].getVoisins()[2];
    faces[idTriangle].getVoisins()[2] = -1;
    int st = faces.size()+1;
    faces.push_back(Triangle(p1, faces[idTriangle].getSommets()[0], oldp1, oldt2, st, idTriangle));
    faces[idTriangle].getVoisins()[2] = faces.size()-1;

    updateNeighbors(oldt2, idTriangle, faces.size()-1);

    faces.push_back(Triangle(p1, oldp1, faces[idTriangle].getSommets()[2], oldt0, idTriangle, st-1));
    faces[idTriangle].getVoisins()[0] = faces.size()-1;
    sommets[p1].setFace(idTriangle);
    sommets[oldp1].setFace(faces.size()-1);
    //maj point faces[idTriangle].getSommets[1] = p1; pointeur
    updateNeighbors(oldt0, idTriangle, faces.size()-1);

    std::cout << oldt0 << std::endl << std::endl;

}

void maillage2D::updateNeighbors(int idtR, int idtO, int newid){
    if(faces[idtR].getVoisins()[0] == idtO) faces[idtR].getVoisins()[0] = newid;
    else if(faces[idtR].getVoisins()[1] == idtO) faces[idtR].getVoisins()[1] = newid;
    else if(faces[idtR].getVoisins()[2] == idtO) faces[idtR].getVoisins()[2] = newid;

}



void maillage2D::addPointOut(int p0){
    std::vector<std::pair<int, int>> points;
    std::vector<Triangle *> tris;
    circulateur_de_faces circu = faces_incidente(*sommet_begin());

    int nbt = 0;
    circu = circu.debut();

    do{
        int p1, p2;
        p1 = (*circu)->getSommets()[1];
        p2 = (*circu)->getSommets()[2];

        Vector3 vec1 = Vector3(sommets[p0].getPoint(), sommets[p1].getPoint());
        Vector3 vec2 = Vector3(sommets[p0].getPoint(), sommets[p2].getPoint());
        Vector3 dir;
        dir = dir.cross(vec1, vec2);
        if(dir.z > 0){
            nbt++;
            points.push_back(std::make_pair(p1, p2));
            tris.push_back(*circu);

        }
        ++circu;
    }while(circu != circu.debut());

    for(unsigned int i = 0; i < points.size(); i++){


        int oldp1 = tris[i]->getSommets()[1];
        tris[i]->getSommets()[0] = p0;

    }
    int idStart=1;
    std::vector<Triangle *> orderTris;
    std::vector<std::pair<int, int>> orderPoints;
    if(tris.size() > 1){

        Triangle * startT = tris[0];
        for(int i = 1; i < tris.size(); i++){
            if(&(faces[startT->getVoisins()[1]]) == tris[i]){

                idStart++;
             }else break;
            startT = tris[i];
        }
        if(idStart < tris.size()){

            for(int i = idStart; i < tris.size(); i++) {
                orderTris.push_back(tris[i]);
                orderPoints.push_back(points[i]);
            }
            for(int i = 0; i < idStart; i++){
                orderPoints.push_back(points[i]);
                orderTris.push_back(tris[i]);
            }
        }else{
            orderPoints = points;
            orderTris = tris;
        }
    }else{
        orderPoints = points;
        orderTris = tris;
    }



    if(orderPoints.size() > 0){
        std::cout << "suuuuuuuuuuuuuuuuup0 " << idStart << std::endl;
        int st = faces.size()+1;
        int p1, p2;

        int nT2 = orderTris[0]->getVoisins()[2];
        int nT1 = orderTris[orderTris.size()-1]->getVoisins()[1];
        int inT2, inT1;


        if(orderTris.size()>1){
            inT2 = orderTris[1]->getVoisins()[2];
            //triangles[tris[0]->getVoisins()[2]]

        }
        else{
            inT2 = faces[orderTris[0]->getVoisins()[1]].getVoisins()[2];
        }

        if(orderTris.size()>1) {
            inT1 = orderTris[orderTris.size()-2]->getVoisins()[1];
        }
        else{
            inT1 = faces[orderTris[0]->getVoisins()[2]].getVoisins()[1];

        }
        faces[nT2].getVoisins()[1] = st-1;
        faces[nT1].getVoisins()[2] = st;
        orderTris[0]->getVoisins()[2] = st-1;
        orderTris[orderTris.size()-1]->getVoisins()[1] = st;

        p1 = orderPoints[0].first;
        //maj des voisin au deux créérs
        p2 = orderPoints[orderPoints.size()-1].second;
        faces.push_back(Triangle(0, p1, p0, inT2, st, nT2));

        faces.push_back(Triangle(0, p0, p2, inT1, nT1, st-1));

        sommets[0].setFace(faces.size()-2);
        sommets[p0].setFace(inT2);

    }
}

std::pair<int, int> maillage2D::getSommetOppose(int triangleId, int sommetPos){
    int ids = -1;
    int tId;
    int sommmet1Arrete;
    int sommmet2Arrete;
    if(sommetPos == 0) {
        tId = faces[triangleId].getVoisins()[0];
        //std::cout << faces[tId] << "\n" << faces[triangleId] <<std::endl;
        sommmet1Arrete = faces[triangleId].getSommets()[1];
        sommmet2Arrete = faces[triangleId].getSommets()[2];
    }
    else if(sommetPos == 1) {
        tId = faces[triangleId].getVoisins()[1];

        //std::cout << faces[tId] << "\n" << faces[triangleId] <<std::endl;
        sommmet1Arrete = faces[triangleId].getSommets()[2];
        sommmet2Arrete = faces[triangleId].getSommets()[0];
    }
    else {
        tId = faces[triangleId].getVoisins()[2];
        sommmet1Arrete = faces[triangleId].getSommets()[0];
        sommmet2Arrete = faces[triangleId].getSommets()[1];
    }
    if(faces[tId].getSommets()[0] != sommmet1Arrete && faces[tId].getSommets()[0] != sommmet2Arrete) ids = faces[tId].getSommets()[0];
    else if(faces[tId].getSommets()[1] != sommmet1Arrete && faces[tId].getSommets()[1] != sommmet2Arrete) ids = faces[tId].getSommets()[1];
    else ids = faces[tId].getSommets()[2];
    return std::make_pair(ids, tId);
}

void maillage2D::makeDelauney(){
    bool restart = true;
    while(restart){
    for (int i = 0; i < faces.size(); ++i) {
        restart = false;
        if(faces[i].getSommets()[0] != 0 && faces[i].getSommets()[1] != 0 && faces[i].getSommets()[2] != 0){
            int currentSommet1 = getSommetOppose(i, 0).first;
            int currentVoisin1 = getSommetOppose(i, 0).second;
            int currentSommet2 = getSommetOppose(i, 1).first;
            int currentVoisin2 = getSommetOppose(i, 1).second;
            int currentSommet3 = getSommetOppose(i, 2).first;
            int currentVoisin3 = getSommetOppose(i, 2).second;
            Delaunay d;
            if(currentSommet1 != 0 && !d.isOutCircle(sommets[faces[i].getSommets()[0]].getPoint(), sommets[faces[i].getSommets()[1]].getPoint(), sommets[faces[i].getSommets()[2]].getPoint(), sommets[currentSommet1].getPoint())){
                //std::cout << "swap1 " << currentSommet1 << faces[currentVoisin1] << faces[i].getSommets()[0] << faces[i].getSommets()[1] << faces[i].getSommets()[2] << std::endl;
                if (canSwap(currentVoisin1, i)) swapArete(currentVoisin1, i);
                restart = true;
            }
            if(!restart && currentSommet2 != 0 && !d.isOutCircle(sommets[faces[i].getSommets()[0]].getPoint(), sommets[faces[i].getSommets()[1]].getPoint(), sommets[faces[i].getSommets()[2]].getPoint(), sommets[currentSommet2].getPoint())){
                if (canSwap(currentVoisin2, i)) swapArete(currentVoisin2, i);
                restart = true;
                //std::cout << "swap2 " << currentSommet2 << faces[i].getSommets()[0] << faces[i].getSommets()[1] << faces[i].getSommets()[2] << std::endl;
            }
            if(!restart && currentSommet3 != 0 && !d.isOutCircle(sommets[faces[i].getSommets()[0]].getPoint(), sommets[faces[i].getSommets()[1]].getPoint(), sommets[faces[i].getSommets()[2]].getPoint(), sommets[currentSommet3].getPoint())){
                if (canSwap(currentVoisin3, i)) swapArete(currentVoisin3, i);
                restart = true;
                //std::cout << "swap3 " << currentSommet3 << std::endl;
            }
        }
        if(restart) {
            std::cout << "break" << std::endl;
            break;
        }

    }
    }
}

bool maillage2D::isTrigo(int s1, int s2, int s3){
    Vector3 u(sommets[s1].coord, sommets[s2].coord);
    Vector3 v(sommets[s1].coord, sommets[s3].coord);
    Vector3 uv = Vector3::cross(u, v);
    if(uv.z > 0) {
        return true;
    }
    return false;
}

bool maillage2D::canSwap(int t1, int t2){
    std::pair<int, int> aretesCommunes = somAreteCommune(t1, t2);
    int ac1 = aretesCommunes.first;
    int ac2 = aretesCommunes.second;
    if(ac1 == -1 || ac2 == -1) { return false; }
    if( isTrigo(faces[t1].getSommets()[ac1], faces[t1].getSommets()[(ac1+1)%3], faces[t2].getSommets()[ac2]) &&

        isTrigo(faces[t2].getSommets()[ac2], faces[t2].getSommets()[(ac2+1)%3], faces[t1].getSommets()[ac1]) ) {

        return true;
    }
    return true;
}
