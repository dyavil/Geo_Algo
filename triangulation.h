#ifndef TRIANGULATION
#define TRIANGULATION

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "geometry.h"
#include <GL/gl.h>

template<typename T>
class triangulation {

public:
    //! Constructeur par defaut
    triangulation() : sommets(0), faces(0) {}

    //! Initialiser le maillage a partir d'un .off
    bool load(std::string filename) {

        std::ifstream file(filename.c_str());
        std::string format;
        int nbSommets, nbFaces, nbEdges;
        point3 point;
        int nbSomFace, s1, s2, s3;

        // Check format
        file >> format;
        if(format != "OFF"){
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
            sommets[i] = point3(point.x, point.y, point.z);
        }

        // Initialize faces
        for(int i = 0; i < nbFaces; i++) {
            file >> nbSomFace >> s1 >> s2 >> s3;
            faces[i] = triangle(s1, s2, s3);
        }

        return true;
    }

    //! Affiche le maillage
    void draw() {

        glBegin(GL_TRIANGLES);

        for(unsigned int i = 0; i < faces.size(); i++)
        {
            glVertex3fv(&sommets[faces[i].s1].x);
            glVertex3fv(&sommets[faces[i].s2].x);
            glVertex3fv(&sommets[faces[i].s3].x);
        }

        glEnd();

    }

protected:
    //! Donne le sommet correspondant a une arrête
    int somArete(int tri, int i1, int i2) {

        if(faces[tri].s1 == i1 and faces[tri].s2 == i2) {
            return 2;
        }

        if(faces[tri].s2 == i1 and faces[tri].s3 == i2) {
            return 0;
        }

        if(faces[tri].s3 == i1 and faces[tri].s1 == i2) {
            return 1;
        }

        return -1;
    }

    std::vector<T> sommets;
    std::vector<triangle> faces;

};

#endif // TRIANGULATION

