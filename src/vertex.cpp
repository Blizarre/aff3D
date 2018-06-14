#include "vertex.h"

// element-wise multiply
Vertex Vertex::operator*(const Vertex& v) const {
  float x2, y2, z2;
  x2 = v.x * this->x;
  y2 = v.y * this->y;
  z2 = v.z * this->z;
  return Vertex(x2, y2, z2);
}

// element-wise substraction
Vertex Vertex::operator-(const Vertex& v) const {
  float x2, y2, z2;
  x2 = this->x - v.x;
  y2 = this->y - v.y;
  z2 = this->z - v.z;
  return Vertex(x2, y2, z2);
}

// element-wise division
Vertex Vertex::operator/(const float f) const {
  float x2, y2, z2;
  x2 = this->x / f;
  y2 = this->y / f;
  z2 = this->z / f;
  return Vertex(x2, y2, z2);
}