#include "triangle.h"


/**
 * Applique la transformation t à tous els points du triangle (et à la normale)
 **/
void Triangle::appliquerTransfo(const Transformation t) {
    this->newState[0] = this->points[0].appliquerTransfo(t);
    this->newState[1] = this->points[1].appliquerTransfo(t);
    this->newState[2] = this->points[2].appliquerTransfo(t);
    this->newState[3] = this->points[3].appliquerTransfo(t);
}

/**
 * Détermine si le triangle est face à la caméra (renvoie vrai) ou dans l'autre sens (renvoie faux).
 * Réalisé en regardant le signe de la composante en z de la normale.
 **/
bool Triangle::estAffiche() {
    return (this->newState[3].z) > 0;
}

/**
 * Donne une estimation de la distance entre le triangle et la caméra. Attention, pour des raisons de
 * coût, le résultat n'est pas normé (non divisé par 3)
 **/
float Triangle::distanceMoyenne() const {
    return (this->newState[0].z + this->newState[1].z + this->newState[2].z);
}

