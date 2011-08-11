#include "triangle.h"

#include<iostream>
using namespace std;

void Triangle::appliquerTransfo(Transformation t) {
    this->newState[0] = this->points[0].appliquerTransfo(t);
    this->newState[1] = this->points[1].appliquerTransfo(t);
    this->newState[2] = this->points[2].appliquerTransfo(t);
    this->newState[3] = this->points[3].appliquerTransfo(t);
}

bool Triangle::estAffiche() {
    bool value;
#ifdef WIREFRAME
    value = true;
#else
    value = (this->newState[3].z) > 0 ;
#endif
    return value;
}

float Triangle::distanceMoyenne() const {
    return (this->newState[0].z + this->newState[1].z + this->newState[2].z);
}

