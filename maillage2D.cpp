#include "maillage2D.h"

bool maillage2D::loadOff(std::string filename) {
    std::ifstream file(filename.c_str());
    std::string format;
    int nbSommets, nbFaces, nbEdges;
    Point point;
    int nbSomFace, s1, s2, s3;

    // Check format
    file >> format;
    if(format != "OFF") {
        std::cout << "Not a .off file" << std::endl;
        return false;
    }

    // Resize Vectors
    file >> nbSommets >> nbFaces >> nbEdges;
    sommets.resize(nbSommets);
    faces.resize(nbFaces);

    // Initialize vertexes
    for(int i = 0; i < nbSommets; i++) {
        file >> point.x >> point.y >> point.z;
        sommets[i] = Sommet(point);
    }

    // Initialize faces
    for(int i = 0; i < nbFaces; i++) {
        file >> nbSomFace >> s1 >> s2 >> s3;
        faces[i] = Triangle(s1, s2, s3);
    }

    return true;
}

void maillage2D::drawEdges() {
    for(unsigned int i = 0; i < faces.size(); i++) {
        glBegin(GL_LINE_LOOP);
        glVertex3f(sommets[faces[i].s[0]].coord.x, sommets[faces[i].s[0]].coord.y, sommets[faces[i].s[0]].coord.z);
        glVertex3f(sommets[faces[i].s[1]].coord.x, sommets[faces[i].s[1]].coord.y, sommets[faces[i].s[1]].coord.z);
        glVertex3f(sommets[faces[i].s[2]].coord.x, sommets[faces[i].s[2]].coord.y, sommets[faces[i].s[2]].coord.z);
        glEnd();
    }
}

void maillage2D::drawTriangles() {
    glBegin(GL_TRIANGLES);
    for(unsigned int i = 0; i < faces.size(); i++) {
        glVertex3f(sommets[faces[i].s[0]].coord.x, sommets[faces[i].s[0]].coord.y, sommets[faces[i].s[0]].coord.z);
        glVertex3f(sommets[faces[i].s[1]].coord.x, sommets[faces[i].s[1]].coord.y, sommets[faces[i].s[1]].coord.z);
        glVertex3f(sommets[faces[i].s[2]].coord.x, sommets[faces[i].s[2]].coord.y, sommets[faces[i].s[2]].coord.z);
    }
    glEnd();
}
