#include"vertex.h"


Vertex Vertex::appliquerTransfo(Transformation t) {
    float * coeffs = t.matrix;
    float ox, oy, oz;
    float nx, ny, nz;

    ox = this->x; oy = this->y; oz = this->z;
    nx = ox * coeffs[0] + oy * coeffs[1] + oz * coeffs[2]  + coeffs[3];
    ny = ox * coeffs[4] + oy * coeffs[5] + oz * coeffs[6]  + coeffs[7];
    nz = ox * coeffs[8] + oy * coeffs[9] + oz * coeffs[10] + coeffs[11];
    return Vertex(nx, ny, nz);
}


