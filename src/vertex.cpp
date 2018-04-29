#include"vertex.h"

// element-wise multiply 
Vertex Vertex::operator*(const Vertex v) {
    float x2, y2, z2;
    x2 = v.x * this->x;
    y2 = v.y * this->y;
    z2 = v.z * this->z;
    return Vertex(x2, y2, z2);
}

// element-wise substraction
Vertex Vertex::operator-(const Vertex v) {
    float x2, y2, z2;
    x2 = this->x - v.x;
    y2 = this->y - v.y;
    z2 = this->z - v.z;
    return Vertex(x2, y2, z2);
}
   
// element-wise division
Vertex Vertex::operator/(const float f) {
    float x2, y2, z2;
    x2 = this->x / f;
    y2 = this->y / f;
    z2 = this->z / f;
    return Vertex(x2, y2, z2);
}

// normalization in place
void Vertex::normer() {
    float n  = this->norm();
    this->x /= n;
    this->y /= n;
    this->z /= n;
}

// return the norm of the vector
float Vertex::norm() {
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}



