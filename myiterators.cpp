#include "myiterators.h"


void circulateur_de_faces::getIdSommet(){
    for (unsigned int i = 0; i < mesh->getSommets().size(); ++i) {
        if(mesh->getSommets()[i] == pivot) {
            indexPivot = i;
        }
    }
}


void circulateur_de_sommets::getIdSommet(){
    for (unsigned int i = 0; i < mesh->getSommets().size(); ++i) {
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


Triangle * marche_visibilite::operator *() {
    return &(mesh->getFaces()[current]);
}

Triangle * marche_visibilite::operator ++() {

    index++;

    if(!mesh->isInside(point, current)) {
        for(int i = 0; i < 3; ++i) {
            Point p1 = mesh->getSommets()[mesh->getFaces()[current].getSommets()[(i+1)%3]].getPoint();
            Point p2 = mesh->getSommets()[mesh->getFaces()[current].getSommets()[(i+2)%3]].getPoint();
            if(mesh->isTrigo(p1, point, p2)) {
                current = (mesh->getFaces()[current]).getVoisins()[i];
                break;
            }
        }
    }

    return &(mesh->getFaces()[current]);
}
