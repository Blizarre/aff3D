#include "rasterizer.h"
#include <cassert>
#include <algorithm>

// TODO: Create line Object

// sort 2 numbers
template <typename T>
inline void sort2(T &a, T &b) {
  if (a > b)
    std::swap(a, b);
}


// Sort three points along their y component
void sortInPlace(Point pt[]) {
  if (pt[1].y < pt[0].y) {
    std::swap(pt[0], pt[1]);
  }

  if (pt[2].y < pt[1].y) {
    std::swap(pt[1], pt[2]);
  }

  if (pt[1].y < pt[0].y) {
    std::swap(pt[0], pt[1]);
  }
}

void Rasterizer::drawLine(int start, int end, int y, Uint32 color,
                                 bool isWireFrame) {
  assert(start <= end);
  if (isInRangeY(y) && isLineInRangeX(start, end)) {
    // TODO: drawWireframe should be a separate function
    if (isWireFrame) {
      m_surface.pixel(start, y) = color;
      m_surface.pixel(end, y) = color;
    } else {
      trimXValues(start, end);
      Uint32 *startPosition = &m_surface.pixel(start, y);
      Uint32 *endPosition = &m_surface.pixel(end, y);
      std::fill(startPosition, endPosition, color);
    }
  }
}

/**
 * Clamp start and end so that they fit inside [0, m_surface.getWidth()[
 * preconditions:
 *  - start <= end
 *  - the line [start, end] has at least one pixel inside [0, m_surface.getWidth()[
 **/
void Rasterizer::trimXValues(int &start, int &end) {
  assert(start <= end);
  assert(! (start >= m_surface.getWidth() && end >= m_surface.getWidth()));
  assert(! (start < 0 && end < 0));

  if (start < 0) {
    start = 0;
  }
  if (end >= m_surface.getWidth()) {
    end = m_surface.getWidth() - 1;
  }
}

/**
* A triangle is draw in several steps:
* - lightning is computed
* - perspective correction is applied
* - backface culling is applied if necessary
* - Vertex are projected on the screen space.
* - they are ordered by height.
* - the first half of the triangle is drawn (top point to the middle point)
* - the bottom part of the triangle is drawn
* TODO: This code should be refactored. For now it is used as a blackbox
**/
void Rasterizer::drawTriangle(const Triangle &t, Normal& lightSource, bool isWireFrame, bool backFaceCulling) {
  // We must compute lightning _before_ doing perspective correction
  float lightCoeff = lightSource.dot(t.normal);

  lightCoeff = (lightCoeff > 0 ? lightCoeff : 0);

  Uint32 color = m_surface.getColor(static_cast<Uint8>(t.r * lightCoeff),
    static_cast<Uint8>(t.g * lightCoeff),
    static_cast<Uint8>(t.b * lightCoeff));

  // Perspective correction
  // TODO: refactor to push outside
  std::array<Vertex, 3> pointsAfterPerspectiveCorrection;

  std::transform(std::begin(t.points), std::end(t.points), std::begin(pointsAfterPerspectiveCorrection), [](const Vertex& v) {
    return Vertex{ v.x / v.z, v.y / v.z, v.z};
  });

  if (backFaceCulling && !t.isFacingCamera()) {
    return;
  }

  // TODO: code reuse between the two halves
  Point tP[3];

  bool isNotDrawn = true;

  for (int v = 0; v < 3; v++) {
    projectToScreen(t.points[v], tP[v]);
    if (tP[v].isInRange(1, m_surface.getWidth() - 1, 0, m_surface.getHeight()))
    {
      isNotDrawn = false;
    }
  }

  if (isNotDrawn) // All points are Out-Of-View
    return;

  sortInPlace(tP);

  int x1, x2;
  float a1, a2;
  int yP;

  yP = 0;
  if (tP[0].y == tP[2].y && tP[0].y > 0 &&
      tP[0].y < m_surface.getHeight() - 1) {
    x1 = tP[0].x;
    x2 = tP[2].x;
    sort2(x1, x2);
    drawLine(x1 -1, x2, tP[0].y, color, isWireFrame);
  } else {
    a1 = (tP[0].x - tP[2].x) / ((float)tP[0].y - tP[2].y);
    a2 = (tP[1].x - tP[2].x) / ((float)tP[1].y - tP[2].y);
    for (int y = tP[2].y; y > tP[1].y; y--) {
      x1 = (int)(-a1 * yP) + tP[2].x;
      x2 = (int)(-a2 * yP) + tP[2].x;
      sort2(x1, x2);
      // TODO: implement proper triangle join
      // all lines are widened by 1 px on the left to make sure all trangles are joined
      drawLine(x1 - 1, x2, y, color, isWireFrame);

      yP++;
    }
  }

  yP = 0;
  if (tP[0].y == tP[1].y && tP[0].y > 0 &&
      tP[0].y < m_surface.getHeight() - 1) {
    x1 = tP[0].x;
    x2 = tP[1].x;
    sort2(x1, x2);
    drawLine(x1 -1, x2, tP[0].y, color, isWireFrame);
  } else {
    a1 = (tP[1].x - tP[0].x) / ((float)tP[1].y - tP[0].y);
    a2 = (tP[2].x - tP[0].x) / ((float)tP[2].y - tP[0].y);

    for (int y = tP[0].y; y <= tP[1].y; y++) {
      x1 = (int)(a1 * yP) + tP[0].x;
      x2 = (int)(a2 * yP) + tP[0].x;
      // TODO: implement proper system
      // all lines are widened by 1 px on the lst to make sure all trangles are joined
      sort2(x1, x2);
      drawLine(x1 - 1, x2, y, color, isWireFrame);

      yP++;
    }
  }
}
