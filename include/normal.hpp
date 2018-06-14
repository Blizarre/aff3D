#pragma once

#include <vertex.h>

class Normal : public Vertex {

public:
  Normal() : Vertex() {}
  explicit Normal(const Vertex &vertex) : Vertex(vertex) {}

  Normal(const Vertex &a, const Vertex &b, const Vertex &c) {
    Normal u = static_cast<Normal>(b - a);
    Normal v = static_cast<Normal>(c - a);

    x = u.y * v.z - u.z * v.y, y = u.z * v.x - u.x * v.z,
    z = u.x - v.y - u.y * v.x;
  }

  Normal(float x, float y, float z) : Vertex(x, y, z) {}

  void normInPlace();

  float norm() const;
  float dot(const Normal &other) const;
};