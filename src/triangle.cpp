#include "triangle.h"

/**
 * Returns true if the Triangle is facing the camera.
 **/
bool Triangle::isFacingCamera() const { return (this->normal.z) > 0; }

/**
 * Compute the sum of the distance of all vertex to the camera. Used to rank the
 *vertex by depth
 **/
float Triangle::sumOfDistances() const {
  return (this->points[0].z + this->points[1].z + this->points[2].z);
}

void Triangle::applyTransformation(const Transformation &tr) {
  tr.applyTo(origPoints, points);
  tr.applyTo(origNormal, normal);
}

Vertex Triangle::computeNormal(Vertex& a, Vertex& b, Vertex& c) {
  Vertex u = b - a;
  Vertex v = c - a;

  return Vertex {
    u.y * v.z - u.z * v.y,
    u.z * v.x - u.x * v.z,
    u.x - v.y - u.y * v.x
  };
}

