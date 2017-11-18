#include "maillage2D.h"

// Renvoie un circulateur de sommets autour du sommet v
circulateur_de_sommets maillage2D::sommets_adjacents(Sommet & v){
    circulateur_de_sommets r = circulateur_de_sommets(&faces[v.face], v, this);
    r.getIdSommet();
    return r;
}

// Renvoie un circulateur de face autour du sommet v
circulateur_de_faces maillage2D::faces_incidente(Sommet & v){
    circulateur_de_faces r = circulateur_de_faces(&faces[v.face], v, this);
    r.getIdSommet();
    return r;
}

// Renvoie un itetrateur de face se dirigeant vers le point p
marche_visibilite maillage2D::marche_begin(Point p) {
    marche_visibilite r = marche_visibilite(p, this);
    return r;
}



// Initialisation par défaut du maillage
void maillage2D::initEmpty(){
    Sommet infinite = Sommet(Point(0, 0, -1), -1);
    sommets.push_back(infinite);
    sommets.push_back(Sommet(Point(0, 0.1, 0), -1));
    sommets.push_back(Sommet(Point(0.1, -0.1, 0), -1));
    sommets.push_back(Sommet(Point(-0.1, -0.1, 0), -1));
    buildMaillage();
    buildVoronoiCenters();
    buildCrust();
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

// Initialisation a partir d'un fichier .pts
void maillage2D::loadPoints(std::string filename, bool d3) {
    std::ifstream file;
    Sommet infinite = Sommet(Point(0, 0, -1), -1);
    sommets.push_back(infinite);
    file.open(filename);
    unsigned int nbs;
    file >> nbs;
    float x0, y0, z0;
    for(unsigned int i = 0; i < nbs; i++){
        if(d3) {
            file >> x0 >> y0 >> z0;
        } else {
            file >> x0 >> y0;
            z0 = 0;
        }
        sommets.push_back(Sommet(Point(x0, y0, z0), -1));
    }
    file.close();
    buildMaillage();
}

//
void maillage2D::calculVoisin(std::map<std::pair<int, int>, int> & faceVoisine, int iSom[]) {
    std::map<std::pair<int, int>, int>::iterator it;
    int nbFaces = faces.size()-1;
    // Faces voisines de la face
    for(int i = 0; i < 3; ++i) {
        it = faceVoisine.find(std::make_pair(iSom[i], iSom[(i+1)%3]));
        if(it != faceVoisine.end()) {
            faces[nbFaces].getVoisins()[(i+2)%3] = it->second;
            faces[it->second].getVoisins()[somArete(it->second, iSom[(i+1)%3], iSom[i])] = nbFaces;
        } else {
            faceVoisine.insert(std::pair<std::pair<int, int>, int> (std::make_pair(iSom[(i+1)%3], iSom[i]), nbFaces));
        }
    }
}

// Construit le maillage à partir des points
void maillage2D::buildMaillage(){
    bool first = true;
    std::map<std::pair<int, int>, int> corresp;
    for (unsigned int i = 1; i < sommets.size(); ++i) {
        //premier triangle
        if(first) {
            if(sommets.size() < 4 ) { return; }

            bool trigo = isTrigo(sommets[i].getPoint(), sommets[i+1].getPoint(), sommets[i+2].getPoint());

            if(trigo){
                faces.push_back(Triangle(i, i+1, i+2, 3, 1, 2));
            } else {
                faces.push_back(Triangle(i, i+2, i+1, 3, 2, 1));
            }

            if(sommets[i].getFace() == -1) { sommets[i].setFace(0); }
            if(sommets[i+1].getFace() == -1) { sommets[i+1].setFace(0); }
            if(sommets[i+2].getFace() == -1) { sommets[i+2].setFace(0); }
            if(sommets[0].getFace() == -1) { sommets[0].setFace(faces.size()); }

            for(int j = 0; j < 3; ++j) {
                int temp = (j+2)%3;
                int iSom[] = {0, (int)i+j, (int)i+temp};

                if(trigo) {
                    faces.push_back(Triangle(0, i+j, i+temp, 0, -1, -1));
                } else {
                    faces.push_back(Triangle(0, i+temp, i+j, 0, -1, -1));
                    iSom[1] = i+temp;
                    iSom[2] = i+j;
                }

                calculVoisin(corresp, iSom);
            }

            //connexion au point infini
            i += 2;
            first = false;
        } else {
            //ajout du reste des points
            unsigned int pos = inTriangle(sommets[i].getPoint());

            if(pos == (unsigned int)-1) {
                addPointOut(i);
            } else {
                addPointIn(pos, i);
            }

            makeIncrementDelauney(i);
        }
    }
}

// Exporte les points vers un fichier
void maillage2D::exportToFile(std::string filename){
    std::ofstream file;
    file.open(filename, std::ofstream::out);
    file << sommets.size()-1 << std::endl;
    for(unsigned int i = 1; i < sommets.size(); i++){
        file << sommets[i].getPoint().x << " " << sommets[i].getPoint().y << " " << sommets[i].getPoint().z << std::endl;
    }
    file.close();
}



// Affiche les triangles du maillage (avec le point infini)
void maillage2D::drawTriangles() {
    glBegin(GL_TRIANGLES);
    for(unsigned int i = 0; i < faces.size(); i++) {
        glVertex3f(sommets[faces[i].getSommets()[0]].coord.x, sommets[faces[i].getSommets()[0]].coord.y, sommets[faces[i].getSommets()[0]].coord.z);
        glVertex3f(sommets[faces[i].getSommets()[1]].coord.x, sommets[faces[i].getSommets()[1]].coord.y, sommets[faces[i].getSommets()[1]].coord.z);
        glVertex3f(sommets[faces[i].getSommets()[2]].coord.x, sommets[faces[i].getSommets()[2]].coord.y, sommets[faces[i].getSommets()[2]].coord.z);
    }
    glEnd();
}

// Affiche les lignes du maillage (avec le point infini)
void maillage2D::drawEdges() {
    for(unsigned int i = 0; i < faces.size(); i++) {
        glBegin(GL_LINE_LOOP);
        glVertex3f(sommets[faces[i].getSommets()[0]].coord.x, sommets[faces[i].getSommets()[0]].coord.y, sommets[faces[i].getSommets()[0]].coord.z);
        glVertex3f(sommets[faces[i].getSommets()[1]].coord.x, sommets[faces[i].getSommets()[1]].coord.y, sommets[faces[i].getSommets()[1]].coord.z);
        glVertex3f(sommets[faces[i].getSommets()[2]].coord.x, sommets[faces[i].getSommets()[2]].coord.y, sommets[faces[i].getSommets()[2]].coord.z);
        glEnd();
    }
}

// Affiche les lignes du maillage (sans le point infini)
void maillage2D::drawEdgesTriangulation() {
    for(unsigned int i = 0; i < faces.size(); i++) {
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_LINE_LOOP);
        if(faces[i].getSommets()[0] != 0 && faces[i].getSommets()[1] != 0 && faces[i].getSommets()[2] != 0){
            glVertex3f(sommets[faces[i].getSommets()[0]].coord.x, sommets[faces[i].getSommets()[0]].coord.y, sommets[faces[i].getSommets()[0]].coord.z);
            glVertex3f(sommets[faces[i].getSommets()[1]].coord.x, sommets[faces[i].getSommets()[1]].coord.y, sommets[faces[i].getSommets()[1]].coord.z);
            glVertex3f(sommets[faces[i].getSommets()[2]].coord.x, sommets[faces[i].getSommets()[2]].coord.y, sommets[faces[i].getSommets()[2]].coord.z);
        }
        glEnd();
    }
}

// Affiche les cercles circonscrits
void maillage2D::drawCircle()
{
   float tpi = 3.14159 * 2.0;
   for (unsigned int i = 0; i < faces.size(); ++i) {
       if(faces[i].getSommets()[0] != 0 && faces[i].getSommets()[1] != 0 && faces[i].getSommets()[2] != 0){
           glBegin(GL_LINE_LOOP);
           for (int j=0; j < 60; j++)
           {

              CercleC c = getCenter(i);
              glVertex2f(c.center.x + (c.radius * cos(j * tpi / 60)),c.center.y + (c.radius* sin(j * tpi / 60)));
           }
           glEnd();
       }
   }
}

// Affiche les cellules de Voronoï
void maillage2D::drawVoronoi(){
    for(unsigned int i = 0; i < voronoiCells.size(); i++) {
        glBegin(GL_LINE_LOOP);
        for (unsigned int j = 0; j < voronoiCells[i].getPoints().size(); ++j) {
            glVertex3f(voronoiPoints[voronoiCells[i].getPoints()[j]].x, voronoiPoints[voronoiCells[i].getPoints()[j]].y, voronoiPoints[voronoiCells[i].getPoints()[j]].z);
        }
        glEnd();
    }
}

//
void maillage2D::drawEdgesPreCrust() {
    for(unsigned int i = 0; i < facesPreCrust.size(); i++) {
        if(facesPreCrust[i].getSommets()[0] >= startCrust) glColor3f(0.0, 1.0, 0.0);
        else glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_LINE_LOOP);
        if(facesPreCrust[i].getSommets()[0] != 0 && facesPreCrust[i].getSommets()[1] != 0 && facesPreCrust[i].getSommets()[2] != 0){
            if(facesPreCrust[i].getSommets()[0] >= startCrust) glColor3f(0.0, 1.0, 0.0);
            else glColor3f(1.0, 1.0, 0.0);
            glVertex3f(sommetsPreCrust[facesPreCrust[i].getSommets()[0]].coord.x, sommetsPreCrust[facesPreCrust[i].getSommets()[0]].coord.y, sommetsPreCrust[facesPreCrust[i].getSommets()[0]].coord.z);
            if(facesPreCrust[i].getSommets()[1] >= startCrust) glColor3f(0.0, 1.0, 0.0);
            else glColor3f(1.0, 1.0, 0.0);
            glVertex3f(sommetsPreCrust[facesPreCrust[i].getSommets()[1]].coord.x, sommetsPreCrust[facesPreCrust[i].getSommets()[1]].coord.y, sommetsPreCrust[facesPreCrust[i].getSommets()[1]].coord.z);
            if(facesPreCrust[i].getSommets()[2] >= startCrust) glColor3f(0.0, 1.0, 0.0);
            else glColor3f(1.0, 1.0, 0.0);
            glVertex3f(sommetsPreCrust[facesPreCrust[i].getSommets()[2]].coord.x, sommetsPreCrust[facesPreCrust[i].getSommets()[2]].coord.y, sommetsPreCrust[facesPreCrust[i].getSommets()[2]].coord.z);
        }
        glEnd();
    }
}

// Affiche Crust
void maillage2D::drawCrust(){
    for (unsigned int i = 0; i < sommetsCrust.size(); ++i) {
        glBegin(GL_LINES);
        glVertex3f(sommetsCrust[i].getPoint().x, sommetsCrust[i].getPoint().y, sommetsCrust[i].getPoint().z);
        i++;
        glVertex3f(sommetsCrust[i].getPoint().x, sommetsCrust[i].getPoint().y, sommetsCrust[i].getPoint().z);
        glEnd();
    }
}



// Donne la position du sommet correspondant a une arête
int maillage2D::somArete(int tri, int i1, int i2) {
    for(int i = 0; i < 3; i++) {
       if(faces[tri].getSommets()[i] == i1 && faces[tri].getSommets()[(i+1)%3] == i2) {
           return (i+2)%3;
       }
    }
    return -1;
}

// Donne les indexs de l'arête correspondante a un sommet
std::pair<int, int> maillage2D::areteSommet(int idt, int ids){
    if(ids == faces[idt].getSommets()[0]) {
        return std::make_pair(faces[idt].getSommets()[1], faces[idt].getSommets()[2]);
    } else if (ids == faces[idt].getSommets()[1]) {
        return std::make_pair(faces[idt].getSommets()[2], faces[idt].getSommets()[0]);
    } else if(ids == faces[idt].getSommets()[2]) {
        return std::make_pair(faces[idt].getSommets()[0], faces[idt].getSommets()[1]);
    }
    return std::make_pair(-1, -1);
}

// Donnes les positions de l'arête commune a 2 triangles
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

// Donne l'index du sommet opposé de la position p dans le triangle tId
int maillage2D::getSommetOppose(int tId, int p) {
    int triOppose = faces[tId].getVoisins()[p];
    std::pair<int, int> arete = somAreteCommune(tId, triOppose);
    int idSom = faces[triOppose].getSommets()[arete.second];
    return idSom;
}

// Indique si un triangle a pour sommet le point infini
bool maillage2D::isInvisible(int t) {
    for(int i = 0; i < 3; ++i) {
        if(faces[t].getSommets()[i] == 0) {
            return true;
        }
    }
    return false;
}

// Indique si les points sont dans le sens trigo
bool maillage2D::isTrigo(Point p1, Point p2, Point p3){
    Vector3 u(p1, p2);
    Vector3 v(p1, p3);
    Vector3 uv = Vector3::cross(u, v);
    if(uv.z > 0) {
        return true;
    }
    return false;
}

// Indique si le point p est contenu dans le triangle t
bool maillage2D::isInside(Point & p, int t) {
    Vector3 cross[3];
    for(int i = 0; i < 3; ++i) {
        Vector3 vec1 = Vector3(p, sommets[faces[t].getSommets()[i]].getPoint());
        Vector3 vec2 = Vector3(p, sommets[faces[t].getSommets()[(i+1)%3]].getPoint());
        cross[i] = Vector3::cross(vec1, vec2);
    }
    if(cross[0].z < 0 && cross[1].z < 0 && cross[2].z) {
        return true;
    }
    return false;
}

// Retourne l'index du triangle contenant le point p1
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

// Remet le point infini d'un triangle a la position 0 (inutilisé)
void maillage2D::setInfinyAtZero(int t) {
    if(isInvisible(t)){
        while(faces[t].getSommets()[0] != 0) {
            int i2 = faces[t].getSommets()[2];
            // Màj des faces
            int s2 = faces[t].getSommets()[2];
            faces[t].getSommets()[2] = faces[t].getSommets()[1];
            faces[t].getSommets()[1] = faces[t].getSommets()[0];
            faces[t].getSommets()[0] = i2;
            faces[t].getSommets()[0] = s2;
            // Màj des voisins
            int v2 = faces[t].getVoisins()[2];
            faces[t].getVoisins()[2] = faces[t].getVoisins()[1];
            faces[t].getVoisins()[1] = faces[t].getVoisins()[0];
            faces[t].getVoisins()[0] = v2;
        }
    }
}

//
void maillage2D::updateNeighbors(int idtR, int idtO, int newid){
    if(faces[idtR].getVoisins()[0] == idtO) faces[idtR].getVoisins()[0] = newid;
    else if(faces[idtR].getVoisins()[1] == idtO) faces[idtR].getVoisins()[1] = newid;
    else if(faces[idtR].getVoisins()[2] == idtO) faces[idtR].getVoisins()[2] = newid;
}

// Ajoute un point a l'intérieur du maillage
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
}

// Ajoute un point a l'extérieur du maillage
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
        tris[i]->getSommets()[0] = p0;
    }
    unsigned int idStart=1;
    std::vector<Triangle *> orderTris;
    std::vector<std::pair<int, int>> orderPoints;
    if(tris.size() > 1) {
        Triangle * startT = tris[0];
        for(unsigned int i = 1; i < tris.size(); i++){
            if(&(faces[startT->getVoisins()[1]]) == tris[i]){
                idStart++;
            } else {
                break;
            }
            startT = tris[i];
        }
        if(idStart < tris.size()){
            for(unsigned int i = idStart; i < tris.size(); i++) {
                orderTris.push_back(tris[i]);
                orderPoints.push_back(points[i]);
            }
            for(unsigned int i = 0; i < idStart; i++){
                orderPoints.push_back(points[i]);
                orderTris.push_back(tris[i]);
            }
        } else {
            orderPoints = points;
            orderTris = tris;
        }
    } else {
        orderPoints = points;
        orderTris = tris;
    }

    if(orderPoints.size() > 0){
        int st = faces.size()+1;
        int p1, p2;

        int nT2 = orderTris[0]->getVoisins()[2];
        int nT1 = orderTris[orderTris.size()-1]->getVoisins()[1];
        int inT2, inT1;

        if(orderTris.size()>1){
            inT2 = orderTris[1]->getVoisins()[2];
        } else {
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

// Ajoute un point au maillage
void maillage2D::addPointUI(Point np){
    sommets.push_back(Sommet(np, -1));
    int i = sommets.size()-1;
    unsigned int pos = inTriangle(sommets[i].getPoint());
    if(pos == (unsigned int)-1) {
        addPointOut(i);
    }
    else {
        addPointIn(pos, i);
    }

    makeIncrementDelauney(i);
    buildVoronoiCenters();
    buildCrust();
}

// Indique si 2 triangles sont "swapable" (inutilisé)
bool maillage2D::canSwap(int t1, int t2){
    std::pair<int, int> aretesCommunes = somAreteCommune(t1, t2);
    int ac1 = aretesCommunes.first;
    int ac2 = aretesCommunes.second;
    if(ac1 == -1 || ac2 == -1) { return false; }

    Point t1p1 = sommets[faces[t1].getSommets()[ac1]].getPoint();
    Point t1p2 = sommets[faces[t1].getSommets()[(ac1+1)%3]].getPoint();
    //Point t1p3 = sommets[faces[t2].getSommets()[(ac2+1)%3]].getPoint();
    Point t2p1 = sommets[faces[t2].getSommets()[ac2]].getPoint();
    Point t2p2 = sommets[faces[t2].getSommets()[(ac2+1)%3]].getPoint();
    //Point t2p3 = sommets[faces[t1].getSommets()[(ac1+1)%3]].getPoint();
    if(isTrigo(t1p1, t1p2, t2p1) && isTrigo(t2p1, t2p2, t1p1)) { return true; }

    return false;
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
    faces[t1].getVoisins()[(ac1+2)%3] = copyT1.getVoisins()[(ac1+2)%3];
    faces[t2].getVoisins()[ac2] = copyT1.getVoisins()[(ac1+1)%3];
    faces[t2].getVoisins()[(ac2+1)%3] = t1;
    faces[t2].getVoisins()[(ac2+2)%3] = copyT2.getVoisins()[(ac2+2)%3];

    // Mise a jour des faces voisines des voisins
    faces[copyT2.getVoisins()[(ac2+1)%3]].getVoisins()[somArete(copyT2.getVoisins()[(ac2+1)%3], copyT2.getSommets()[ac2], copyT2.getSommets()[(ac2+2)%3])] = t1;
    faces[copyT1.getVoisins()[(ac1+1)%3]].getVoisins()[somArete(copyT1.getVoisins()[(ac1+1)%3], copyT1.getSommets()[ac1], copyT1.getSommets()[(ac1+2)%3])] = t2;

    // Mise a jour des faces pointées par les sommets
    sommets[faces[t1].getSommets()[(ac1+1)%3]].face = t1;
    sommets[faces[t2].getSommets()[(ac2+1)%3]].face = t2;

    // On répare les triangles "invisibles"
    /*
    setInfinyAtZero(t1);
    setInfinyAtZero(t2);
    setInfinyAtZero(copyT2.getVoisins()[(ac2+1)%3]);
    setInfinyAtZero(copyT1.getVoisins()[(ac1+1)%3]);
    */
}



void maillage2D::makeDelauney(){
    bool restart = true;
    while(restart){
        for (unsigned int i = 0; i < faces.size(); ++i) {
            restart = false;
            if(faces[i].getSommets()[0] != 0 && faces[i].getSommets()[1] != 0 && faces[i].getSommets()[2] != 0){
                int currentSommet1 = getSommetOppose(i, 0);
                int currentVoisin1 = faces[i].getVoisins()[0];

                int currentSommet2 = getSommetOppose(i, 1);
                int currentVoisin2 = faces[i].getVoisins()[1];

                int currentSommet3 = getSommetOppose(i, 2);
                int currentVoisin3 = faces[i].getVoisins()[2];

                Delaunay d;

                if(currentSommet1 != 0 && !d.isOutCircle(sommets[faces[i].getSommets()[0]].getPoint(), sommets[faces[i].getSommets()[1]].getPoint(), sommets[faces[i].getSommets()[2]].getPoint(), sommets[currentSommet1].getPoint())) {
                    if (canSwap(currentVoisin1, i)) swapArete(currentVoisin1, i);
                    restart = true;
                }
                if(!restart && currentSommet2 != 0 && !d.isOutCircle(sommets[faces[i].getSommets()[0]].getPoint(), sommets[faces[i].getSommets()[1]].getPoint(), sommets[faces[i].getSommets()[2]].getPoint(), sommets[currentSommet2].getPoint())) {
                    if (canSwap(currentVoisin2, i)) swapArete(currentVoisin2, i);
                    restart = true;
                }
                if(!restart && currentSommet3 != 0 && !d.isOutCircle(sommets[faces[i].getSommets()[0]].getPoint(), sommets[faces[i].getSommets()[1]].getPoint(), sommets[faces[i].getSommets()[2]].getPoint(), sommets[currentSommet3].getPoint())) {
                    if (canSwap(currentVoisin3, i)) swapArete(currentVoisin3, i);
                    restart = true;
                }
            }
            if(restart) { break; }
        }
    }
}

void maillage2D::makeIncrementDelauney(int s){
    std::list<std::pair<int, int>> arretes;
    std::set<std::pair<int, int>> done;
    std::vector<int> initTriangles;
    circulateur_de_faces circu = faces_incidente(sommets[s]);
    do {
        int t = *circu - &faces[0];
        initTriangles.push_back(t);
        ++circu;
    } while(circu != circu.debut());
    for(unsigned int i = 0; i < initTriangles.size(); i++) {
        std::pair<int, int> tmp = areteSommet(initTriangles[i], s);
        int pos = somArete(initTriangles[i], tmp.first, tmp.second);
        if(!isInvisible(initTriangles[i]) && !isInvisible(faces[initTriangles[i]].getVoisins()[pos])) {
            arretes.push_back(std::make_pair(initTriangles[i], faces[initTriangles[i]].getVoisins()[pos]));
        }
    }
    while(!arretes.empty()) {
        std::pair<int, int> current = arretes.front();
        arretes.pop_front();
        Delaunay d;
        int pos = somAreteCommune(current.first, current.second).second;
        auto it = done.end();
        if(!isInvisible(current.first) && !isInvisible(current.second) && !d.isOutCircle(sommets[faces[current.first].getSommets()[0]].getPoint(), sommets[faces[current.first].getSommets()[1]].getPoint(), sommets[faces[current.first].getSommets()[2]].getPoint(), sommets[faces[current.second].getSommets()[pos]].getPoint())) {
            if(canSwap(current.first, current.second)) {
                int v1 = faces[current.second].getVoisins()[(pos+1)%3];
                int v2 = faces[current.second].getVoisins()[(pos+2)%3];
                swapArete(current.first, current.second);
                done.insert(std::pair<int, int> (std::make_pair(std::min(current.first, current.second), std::max(current.first, current.second))));
                if(!isInvisible(v1) && done.find(std::make_pair(std::min(current.first, v1), std::max(current.first, v1))) == it) arretes.push_back(std::make_pair(current.first, v1));
                if(!isInvisible(v2) && done.find(std::make_pair(std::min(current.second, v2), std::max(current.second, v2))) == it) arretes.push_back(std::make_pair(current.second, v2));
            }
        }
    }
}

// Indique si le maillage est globalement de Delaunay
bool maillage2D::checkDelaunay(){
    bool res = true;
    for (unsigned int i = 0; i < faces.size(); ++i) {
        if(faces[i].getSommets()[0] != 0 && faces[i].getSommets()[1] != 0 && faces[i].getSommets()[2] != 0){
            int currentSommet1 = getSommetOppose(i, 0);
            int currentSommet2 = getSommetOppose(i, 1);
            int currentSommet3 = getSommetOppose(i, 2);
            Delaunay d;
            if(currentSommet1 != 0 && !d.isOutCircle(sommets[faces[i].getSommets()[0]].getPoint(), sommets[faces[i].getSommets()[1]].getPoint(), sommets[faces[i].getSommets()[2]].getPoint(), sommets[currentSommet1].getPoint())){
                res = false;
            }
            if(currentSommet2 != 0 && !d.isOutCircle(sommets[faces[i].getSommets()[0]].getPoint(), sommets[faces[i].getSommets()[1]].getPoint(), sommets[faces[i].getSommets()[2]].getPoint(), sommets[currentSommet2].getPoint())){
                res = false;
            }
            if(currentSommet3 != 0 && !d.isOutCircle(sommets[faces[i].getSommets()[0]].getPoint(), sommets[faces[i].getSommets()[1]].getPoint(), sommets[faces[i].getSommets()[2]].getPoint(), sommets[currentSommet3].getPoint())){
                res = false;
            }
        }
    }

    for (unsigned int i = 0; i < faces.size(); ++i) {
        if(faces[i].getSommets()[0] != 0 && faces[i].getSommets()[1] != 0 && faces[i].getSommets()[2] != 0){
            CercleC cer = getCenter(i);
            Point cmp;
            for (unsigned int j = 1; j < sommets.size(); ++j) {
                if(j != (unsigned int)faces[i].getSommets()[0] && j != (unsigned int)faces[i].getSommets()[1] && j != (unsigned int)faces[i].getSommets()[2]){
                    if(cmp.dist(cer.center, sommets[j].coord) < cer.radius) res = false;
                }
            }
        }
    }
    return res;
}

//
void maillage2D::buildVoronoiCenters(){
    voronoiPoints.clear();
    voronoiCells.clear();
    for (unsigned int i = 0; i < faces.size(); ++i) {
        CercleC cer = getCenter(i);
        voronoiPoints.push_back(cer.center);
    }
    sommet_iterator iit = sommet_begin();
    while (iit != sommet_end()) {
        VoronoiCell cell;
        circulateur_de_faces circu = faces_incidente(*iit);

        circu = circu.debut();
        do {
            if((*circu)->getSommets()[0] != 0 && (*circu)->getSommets()[1] != 0 && (*circu)->getSommets()[2] != 0){
               int idt = *circu - &faces[0];
               cell.getPoints().push_back(idt);
            }
           ++circu;

        } while(circu!=circu.debut());
        voronoiCells.push_back(cell);
        iit++;
    }
}

//
void maillage2D::buildCrust(){
    std::map<std::pair<int, int>, int> r;
    std::vector<Sommet> tmp = sommets;
    std::vector<Triangle> save = faces;
    sommetsCrust.clear();
    int start = sommets.size();
    for (unsigned int i = 0; i < voronoiPoints.size(); ++i) {
        if(faces[i].getSommets()[0] != 0 && faces[i].getSommets()[1] != 0 && faces[i].getSommets()[2] != 0) {
            sommets.push_back(Sommet(voronoiPoints[i]));
        }
    }

    for (unsigned int i = start; i < sommets.size(); ++i) {
        unsigned int pos = inTriangle(sommets[i].getPoint());
        if(pos == (unsigned int)-1) {
            addPointOut(i);
        } else {
            addPointIn(pos, i);
        }
        makeIncrementDelauney(i);
    }

    for (unsigned int i = 0; i < faces.size(); ++i) {
        if(faces[i].getSommets()[0] != 0 && faces[i].getSommets()[1] != 0 && faces[i].getSommets()[2] != 0) {
            int id1 = std::max(faces[i].getSommets()[0], faces[i].getSommets()[1]);
            int id2 = std::min(faces[i].getSommets()[0], faces[i].getSommets()[1]);
            bool cont = false;
            if(id2 < start && id1 < start){
                cont = true;
            }
            auto it = r.find(std::make_pair(id1, id2));
            if(cont && it == r.end()){
                r.insert(std::pair<std::pair<int, int>, int> (std::make_pair(id1, id2), 0));
                sommetsCrust.push_back(sommets[faces[i].getSommets()[0]]);
                sommetsCrust.push_back(sommets[faces[i].getSommets()[1]]);
            }
            id1 = std::max(faces[i].getSommets()[1], faces[i].getSommets()[2]);
            id2 = std::min(faces[i].getSommets()[1], faces[i].getSommets()[2]);
            cont = false;
            if(id2 < start && id1 < start) { cont = true; }
            it = r.find(std::make_pair(id1, id2));
            if(cont && it == r.end()) {
                r.insert(std::pair<std::pair<int, int>, int> (std::make_pair(id1, id2), 0));
                sommetsCrust.push_back(sommets[faces[i].getSommets()[1]]);
                sommetsCrust.push_back(sommets[faces[i].getSommets()[2]]);
            }
            id1 = std::max(faces[i].getSommets()[2], faces[i].getSommets()[0]);
            id2 = std::min(faces[i].getSommets()[2], faces[i].getSommets()[0]);
            cont = false;
            if(id2 < start && id1 < start) { cont = true; }
            it = r.find(std::make_pair(id1, id2));
            if(cont && it == r.end()) {
                r.insert(std::pair<std::pair<int, int>, int> (std::make_pair(id1, id2), 0));
                sommetsCrust.push_back(sommets[faces[i].getSommets()[2]]);
                sommetsCrust.push_back(sommets[faces[i].getSommets()[0]]);
            }
        }
    }
    sommetsPreCrust = sommets;
    startCrust = start;
    facesPreCrust = faces;
    faces = save;
    sommets = tmp;
}

CercleC maillage2D::getCenter(int idt){
    Point tp1, tp2, tp3;
    Point res;
    CercleC ret;
    float d;
    tp1 = sommets[faces[idt].getSommets()[0]].getPoint();
    tp2 = sommets[faces[idt].getSommets()[1]].getPoint();
    tp3 = sommets[faces[idt].getSommets()[2]].getPoint();
    Vector3 a(tp1);
    Vector3 b(tp2);
    Vector3 c(tp3);
    d = 2 *(a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y));
    res.x = (1/d)*((a.x*a.x+a.y*a.y)*(b.y-c.y)+(b.x*b.x+b.y*b.y)*(c.y-a.y)+(c.x*c.x+c.y*c.y)*(a.y-b.y));
    res.y = (1/d)*((a.x*a.x+a.y*a.y)*(c.x-b.x)+(b.x*b.x+b.y*b.y)*(a.x-c.x)+(c.x*c.x+c.y*c.y)*(b.x-a.x));
    ret.center = res;
    float rad = sqrt((a.x-res.x)*(a.x-res.x)+(a.y-res.y)*(a.y-res.y));
    ret.radius = rad;
    return ret;
}

CercleC maillage2D::getCenter(Triangle *idt){
    Point tp1, tp2, tp3;
    Point res;
    CercleC ret;
    float d;
    tp1 = sommets[idt->getSommets()[0]].getPoint();
    tp2 = sommets[idt->getSommets()[1]].getPoint();
    tp3 = sommets[idt->getSommets()[2]].getPoint();
    Vector3 a(tp1);
    Vector3 b(tp2);
    Vector3 c(tp3);
    d = 2 *(a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y));
    res.x = (1/d)*((a.x*a.x+a.y*a.y)*(b.y-c.y)+(b.x*b.x+b.y*b.y)*(c.y-a.y)+(c.x*c.x+c.y*c.y)*(a.y-b.y));
    res.y = (1/d)*((a.x*a.x+a.y*a.y)*(c.x-b.x)+(b.x*b.x+b.y*b.y)*(a.x-c.x)+(c.x*c.x+c.y*c.y)*(b.x-a.x));
    ret.center = res;
    float rad = sqrt((a.x-res.x)*(a.x-res.x)+(a.y-res.y)*(a.y-res.y));
    ret.radius = rad;
    return ret;
}
