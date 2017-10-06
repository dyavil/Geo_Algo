#include "myiterators.h"


void circulateur_de_faces::getIdSommet(){
    for (int i = 0; i < mesh->getSommets().size(); ++i) {
        if(mesh->getSommets()[i] == pivot) {
            indexPivot = i;
        }
    }
}


void circulateur_de_sommets::getIdSommet(){
    for (int i = 0; i < mesh->getSommets().size(); ++i) {
        if(mesh->getSommets()[i] == pivot) {
            indexPivot = i;
        }
    }
    if(currentT->getSommets()[0] == indexPivot) {
        startP = &(mesh->getSommets()[currentT->getSommets()[1]]);;
    }
    else if(currentT->getSommets()[1] == indexPivot) {
        startP = &(mesh->getSommets()[currentT->getSommets()[2]]);;
    }
    else {
        startP = &(mesh->getSommets()[currentT->getSommets()[0]]);;
    }
    current = startP;

}

circulateur_de_sommets circulateur_de_sommets::operator =(Sommet* s){
    current = s;
    return *this;
}


Sommet * circulateur_de_sommets::debut(){
    return startP;
}



Triangle * circulateur_de_faces::operator ++(){

    if(current->getSommets()[0] == indexPivot) {
        current = &(mesh->getFaces()[current->getVoisins()[1]]);
    }
    else if(current->getSommets()[1] == indexPivot) {
        current = &(mesh->getFaces()[current->getVoisins()[2]]);
    }
    else {
        current = &(mesh->getFaces()[current->getVoisins()[0]]);
    }

    return current;
}


Sommet * circulateur_de_sommets::operator ++(){

    if(currentT->getSommets()[0] == indexPivot) {
        currentT = &(mesh->getFaces()[currentT->getVoisins()[1]]);
    }
    else if(currentT->getSommets()[1] == indexPivot) {
        currentT = &(mesh->getFaces()[currentT->getVoisins()[2]]);
    }
    else {
        currentT = &(mesh->getFaces()[currentT->getVoisins()[0]]);
    }
    if(currentT->getSommets()[0] == indexPivot) {
        current = &(mesh->getSommets()[currentT->getSommets()[1]]);
    }
    else if(currentT->getSommets()[1] == indexPivot) {
        current = &(mesh->getSommets()[currentT->getSommets()[2]]);
    }
    else {
        current = &(mesh->getSommets()[currentT->getSommets()[0]]);
    }

    return current;
}
