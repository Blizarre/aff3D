#include "normal.h"

void Normal::normInPlace() {
  float n = this->norm();
  this->x /= n;
  this->y /= n;
  this->z /= n;
}

// return the norm of the vector
float Normal::norm() const {
  return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

float Normal::dot(const Normal &v) const { return x * v.x + y * v.y + z * v.z; }

Normal::Normal(const Vertex &a, const Vertex &b, const Vertex &c) {
  Normal u = static_cast<Normal>(b - a);
  Normal v = static_cast<Normal>(c - a);

  x = u.y * v.z - u.z * v.y;
  y = u.z * v.x - u.x * v.z;
  z = u.x * v.y - u.y * v.x;

  normInPlace();
}
