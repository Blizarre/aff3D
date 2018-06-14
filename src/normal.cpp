#include <normal.hpp>

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

float Normal::dot(const Normal& v) const {
    return x * v.x + y * v.y + z * v.z;
}