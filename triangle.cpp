#include "triangle.h"

void Triangle::appliquerTransfo(Transformation t) {
    this->newState[0] = this->points[0].appliquerTransfo(t);
    this->newState[1] = this->points[1].appliquerTransfo(t);
    this->newState[2] = this->points[2].appliquerTransfo(t);
}


