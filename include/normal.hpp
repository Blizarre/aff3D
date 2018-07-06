#pragma once

#include <vertex.h>

class Normal : public Vertex {

public:
  Normal() : Vertex() {}
  explicit Normal(const Vertex &vertex) : Vertex(vertex) {}

  Normal(const Vertex &a, const Vertex &b, const Vertex &c);

  Normal(float x, float y, float z) : Vertex(x, y, z) {}

  void normInPlace();

  float norm() const;
  float dot(const Normal &other) const;
};