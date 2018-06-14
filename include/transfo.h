#pragma once

#include <algorithm>
#include <array>
#include <vector>

#include "vertex.h"
#include "normal.hpp"

class Transformation {

public:
  Transformation() {
    // Initialization with unity matrix (all 0, except the diagonal filled with
    // 1)
    m_matrix.fill(0.0f);

    m_matrix[0] = 1.0f;
    m_matrix[5] = 1.0f;
    m_matrix[10] = 1.0f;
    m_matrix[15] = 1.0f;
  }

  void translate(const float delta[3]);
  void translate(const std::array<float, 3>& delta);

  void rotationX(const float rot);
  void rotationY(const float rot);
  void rotationZ(const float rot);

  Vertex applyTo(const Vertex &v) const;
  Normal applyTo(const Normal &n) const;
  void Transformation::applyTo(const Vertex &vIn, Vertex &vOut) const;

  /***
    * Apply the transformation to all elements of the array (size N).
  ***/
  template <typename Type, size_t N>
  void applyTo(const std::array<Type, N> &originalArray,
    std::array<Type, N> &newArray) const {
    auto itOrig = originalArray.cbegin();
    auto itNew = newArray.begin();

    for (; itOrig != originalArray.cend(); ++itOrig, ++itNew) {
      applyTo(*itOrig, *itNew);
    }
  };

protected:
  std::array<float, 16> m_matrix;
};