#pragma once

#include "triangle.h"
#include "utils.h"
#include <limits>
#include <vector>

class TriangleNormalizer {
public:
  template <class Iter> TriangleNormalizer(Iter begin, Iter end);

  template <class RandomAccessIterator>
  std::vector<Triangle> normalize(RandomAccessIterator begin,
                                  RandomAccessIterator end);

private:
  float minX = std::numeric_limits<float>::max();
  float maxX = std::numeric_limits<float>::min();
  float minY = std::numeric_limits<float>::max();
  float maxY = std::numeric_limits<float>::min();
  float minZ = std::numeric_limits<float>::max();
  float maxZ = std::numeric_limits<float>::min();

  float scale;
};

/**
    Template implementation
**/

template <class Iter>
inline TriangleNormalizer::TriangleNormalizer(Iter begin, Iter end) {
  while (begin != end) {
    for (int i = 0; i < 3; i++) {
      minX = MIN(minX, begin->points[i].x);
      minY = MIN(minY, begin->points[i].y);
      minZ = MIN(minZ, begin->points[i].z);
      maxX = MIN(maxX, begin->points[i].x);
      maxY = MIN(maxY, begin->points[i].y);
      maxZ = MIN(maxZ, begin->points[i].z);
    }
    begin++;
  }
  scale = MAX3(maxX - minX, maxY - minY, maxZ - minZ);
}

template <class RandomAccessIterator>
inline std::vector<Triangle>
TriangleNormalizer::normalize(RandomAccessIterator begin,
                              RandomAccessIterator end) {
  std::vector<Triangle> normalized(end - begin);
  auto normedIt = normalized.begin();

  while (begin != end) {
    Triangle t = *begin;
    for (int i = 0; i < 3; i++) {
      Vertex &v = t.points[i];
      v.x -= (maxX - minX) / 2.0f;
      v.y -= (maxY - minY) / 2.0f;
      v.z -= (maxZ - minZ) / 2.0f;

      v.x /= scale;
      v.y /= scale;
      v.z /= scale;
    }
    *normedIt = t;
    begin++;
    normedIt++;
  }
  return normalized;
}
