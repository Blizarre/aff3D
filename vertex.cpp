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

Vertex Vertex::operator*(Vertex v) {
    float x, y, z;
    x = v.x * this->x;
    y = v.y * this->y;
    z = v.z * this->z;
    return Vertex(x, y, z);
}

Vertex Vertex::operator-(Vertex v) {
    float x, y, z;
    x = this->x - v.x;
    y = this->y - v.y;
    z = this->z - v.z;
    return Vertex(x, y, z);
}
    
Vertex Vertex::operator/(float f) {
    float x, y, z;
    x = this->x / f;
    y = this->y / f;
    z = this->z / f;
    return Vertex(x, y, z);
}

void Vertex::normer() {
    float n  = this->norme();
    this->x /= n;
    this->y /= n;
    this->z /= n;
}

float Vertex::norme() {
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}



