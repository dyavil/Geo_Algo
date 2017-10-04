#ifndef TRIANGULATIONTOPO
#define TRIANGULATIONTOPO

#include "triangulation.h"
#include <array>
#include <map>

template<typename T>
class triangulationTopo : public triangulation<T> {

public:
    //! Constructeur par defaut
    triangulationTopo() : voisinSommet(0), voisinsTriangle(0) {}

    //! Initialiser le maillage a partir d'un .off
    bool load(std::string filename) {

        std::ifstream file(filename.c_str());
        std::string format;
        int nbSommets, nbFaces, nbEdges;
        point3 point;
        int nbSomFace, iSom[3];
        std::map<std::pair<int, int>, int> faceVoisine;
        std::map<std::pair<int, int>, int>::iterator it;

        // Check format
        file >> format;
        if(format != "OFF"){
            std::cout << "Not a .off file" << std::endl;
            return false;
        }

        // Resize Vectors
        file >> nbSommets >> nbFaces >> nbEdges;
        triangulation<T>::sommets.resize(nbSommets);
        triangulation<T>::faces.resize(nbFaces);
        voisinSommet.resize(nbSommets, -1);
        voisinsTriangle.resize(nbFaces, {-1, -1, -1});

        // Initialize vertexes
        for(int i = 0; i < nbSommets; i++) {
            file >> point.x >> point.y >> point.z;
            triangulation<T>::sommets[i] = point3(point.x, point.y, point.z);
        }

        // Initialize faces
        for(int i = 0; i < nbFaces; i++) {
            file >> nbSomFace >> iSom[0] >> iSom[1] >> iSom[2];
            triangulation<T>::faces[i] = triangle(iSom[0], iSom[1], iSom[2]);

            // Neighbors of vertexes
            for(int j = 0; j < 3; j++) {
                if(voisinSommet[iSom[j]] == -1) {
                    voisinSommet[iSom[j]] = i;
                }
            }

            // Neighbors of faces
            it = faceVoisine.find(std::make_pair(iSom[0], iSom[1]));
            if(it != faceVoisine.end()){
                voisinsTriangle[i][2] = it->second;
                voisinsTriangle[it->second][triangulation<T>::somArete(it->second, iSom[1], iSom[0])] = i;
            } else {
                faceVoisine.insert(std::pair<std::pair<int, int>, int> (std::make_pair(iSom[1], iSom[0]), i));
            }

            it = faceVoisine.find(std::make_pair(iSom[1], iSom[2]));
            if(it != faceVoisine.end()){
                voisinsTriangle[i][0] = it->second;
                voisinsTriangle[it->second][triangulation<T>::somArete(it->second, iSom[2], iSom[1])] = i;
            } else {
                faceVoisine.insert(std::pair<std::pair<int, int>, int> (std::make_pair(iSom[2], iSom[1]), i));
            }

            it = faceVoisine.find(std::make_pair(iSom[2], iSom[0]));
            if(it != faceVoisine.end()){
                voisinsTriangle[i][1] = it->second;
                voisinsTriangle[it->second][triangulation<T>::somArete(it->second, iSom[0], iSom[2])] = i;
            } else {
                faceVoisine.insert(std::pair<std::pair<int, int>, int> (std::make_pair(iSom[0], iSom[2]), i));
            }

        }

        return true;
    }

protected:
    std::vector<int> voisinSommet;
    std::vector<std::array<int, 3>> voisinsTriangle;

};

#endif // TRIANGULATIONTOPO

