#include "maillage2D.h"


// Donne le sommet correspondant a une arête
int maillage2D::somArete(int tri, int i1, int i2) {
    for(int i = 0; i < 3; i++) {
       if(faces[tri].s[i] == i1 && faces[tri].s[(i+1)%3] == i2) {
           return (i+2)%3;
       }
    }
    return -1;
}


// Donnes les sommets des arêtes communes a 2 triangles
std::pair<int, int> maillage2D::somAreteCommune(int t1, int t2) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(faces[t1].s[i] == faces[t2].s[(j+1)%3] && faces[t1].s[(i+1)%3] == faces[t2].s[j]) {
                return std::pair<int, int> ((i+2)%3, (j+2)%3);
            }
        }
    }
    return std::pair<int, int> (-1, -1);
}


// Swap l'arête commune entre 2 triangles
void maillage2D::swapArete(int t1, int t2) {
    Triangle copyT1 = faces[t1];
    Triangle copyT2 = faces[t2];

    std::pair<int, int> aretesCommunes = somAreteCommune(t1, t2);
    int ac1 = aretesCommunes.first;
    int ac2 = aretesCommunes.second;

    // Mise a jour des faces
    faces[t1].s[(ac1+2)%3] = copyT2.s[ac2];
    faces[t2].s[(ac2+2)%3] = copyT1.s[ac1];

    // Mise a jour des voisins
    faces[t1].v[ac1] = copyT2.v[(ac2+1)%3];
    faces[t1].v[(ac1+1)%3] = t2;
    faces[t1].v[(ac1+2)%3] == copyT1.v[(ac1+2)%3];
    faces[t2].v[ac2] = copyT1.v[(ac1+1)%3];
    faces[t2].v[(ac2+1)%3] = t1;
    faces[t2].v[(ac2+2)%3] = copyT2.v[(ac2+2)%3];

    // Mise a jour des voisins des voisins
    faces[copyT2.v[(ac2+1)%3]].v[somArete(copyT2.v[(ac2+1)%3], copyT2.s[ac2], copyT2.s[(ac2+2)%3])] = t1;
    faces[copyT1.v[(ac1+1)%3]].v[somArete(copyT1.v[(ac1+1)%3], copyT1.s[ac1], copyT1.s[(ac1+2)%3])] = t2;
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
            faces[i].v[2] = it->second;
            faces[it->second].v[somArete(it->second, iSom[1], iSom[0])] = i;
        } else {
            faceVoisine.insert(std::pair<std::pair<int, int>, int> (std::make_pair(iSom[1], iSom[0]), i));
        }

        it = faceVoisine.find(std::make_pair(iSom[1], iSom[2]));
        if(it != faceVoisine.end()) {
            faces[i].v[0] = it->second;
            faces[it->second].v[somArete(it->second, iSom[2], iSom[1])] = i;
        } else {
            faceVoisine.insert(std::pair<std::pair<int, int>, int> (std::make_pair(iSom[2], iSom[1]), i));
        }

        it = faceVoisine.find(std::make_pair(iSom[2], iSom[0]));
        if(it != faceVoisine.end()) {
            faces[i].v[1] = it->second;
            faces[it->second].v[somArete(it->second, iSom[0], iSom[2])] = i;
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
        glVertex3f(sommets[faces[i].s[0]].coord.x, sommets[faces[i].s[0]].coord.y, sommets[faces[i].s[0]].coord.z);
        glVertex3f(sommets[faces[i].s[1]].coord.x, sommets[faces[i].s[1]].coord.y, sommets[faces[i].s[1]].coord.z);
        glVertex3f(sommets[faces[i].s[2]].coord.x, sommets[faces[i].s[2]].coord.y, sommets[faces[i].s[2]].coord.z);
        glEnd();
    }
}


// Affiche les triangles du maillage
void maillage2D::drawTriangles() {
    glBegin(GL_TRIANGLES);
    for(unsigned int i = 0; i < faces.size(); i++) {
        glVertex3f(sommets[faces[i].s[0]].coord.x, sommets[faces[i].s[0]].coord.y, sommets[faces[i].s[0]].coord.z);
        glVertex3f(sommets[faces[i].s[1]].coord.x, sommets[faces[i].s[1]].coord.y, sommets[faces[i].s[1]].coord.z);
        glVertex3f(sommets[faces[i].s[2]].coord.x, sommets[faces[i].s[2]].coord.y, sommets[faces[i].s[2]].coord.z);
    }
    glEnd();
}
