#include "rasterizer.h"

// TODO: Create line Object

// sort 2 numbers
inline void sort(int &a, int &b) {
  if (a > b)
    std::swap(a, b);
}

// add one pixel at the left and right of the line to fill gaps between
// triangles
inline void widen(int &min, int &max) {
  min--;
  max++;
}

/*
 Draw the horizontal line between the two points x1 and x2 at height y. Doesn't
 make any boundary check: if x1, x2 or y is invalid,
 memory will be written at an invalid location
*/
inline void Rasterizer::drawLineNoBoundCheck(int x1, int x2, int y,
                                             Uint32 color, bool isWireFrame) {

  if (isWireFrame) {
    m_surface.pixel(x1, y) = color;
    m_surface.pixel(x2, y) = color;
  } else {
    sort(x1, x2);
    widen(x1, x2);
    Uint32 *startPosition = &m_surface.pixel(x1, y);
    Uint32 *endPosition = &m_surface.pixel(x2, y);
    std::fill(startPosition, endPosition, color);
  }
}

/*
Draw the horizontal line between the two points x1 and x2 at height y. Make a
boundary check: if x1, x2 is invalid,
draw only the visible portion of the line. if y is invalid, does not write
anything.
*/
inline void Rasterizer::drawLine(int x1, int x2, int y, Uint32 color,
                                 bool isWireFrame) {
  if (isInRangeY(y)) {
    if (isWireFrame) {
      m_surface.pixel(x1, y) = color;
      m_surface.pixel(x2, y) = color;
    } else {
      sort(x1, x2);
      widen(x1, x2);
      trimXValues(x1, x2);
      Uint32 *startPosition = &m_surface.pixel(x1, y);
      Uint32 *endPosition = &m_surface.pixel(x2, y);
      std::fill(startPosition, endPosition, color);
    }
  }
}

void Rasterizer::trimXValues(int &val1, int &val2) {
  // TODO: use the fact that val1 < val2
  if (val1 < 0)
    val1 = 0;
  else if (val1 >= m_surface.getWidth())
    val1 = m_surface.getWidth() - 1;

  if (val2 < 0)
    val2 = 0;
  else if (val2 >= m_surface.getWidth())
    val2 = m_surface.getWidth() - 1;
}

/**
* A triangle is draw in several steps:
* - first, the Vertex are projected on the screen space.
* - then, they are ordered by height.
* - the first half of the triangle is drawn (top point to the middle point)
* - and finally the bottom part of the triangle is drawn
* TODO: This code should be refactored. For now it is a sandbox
**/
void Rasterizer::drawTriangle(const Triangle &t, bool isWireFrame) {
  // TODO: code reuse between the two halves
  Vertex lightPoint(0.5574f, 0.5574f, 0.5574f);
  float lightCoeff = lightPoint.x * t.points[3].x +
                     lightPoint.y * t.points[3].y +
                     lightPoint.z * t.points[3].z;
  lightCoeff = (lightCoeff > 0 ? lightCoeff : 0);

  Uint32 color = m_surface.getColor(static_cast<Uint8>(t.r * lightCoeff),
                                    static_cast<Uint8>(t.g * lightCoeff),
                                    static_cast<Uint8>(t.b * lightCoeff));

  Point tP[3];

  bool isFullyDrawn = true;
  bool isNotDrawn = true;

  for (int v = 0; v < 3; v++) {
    projectToScreen(t.points[v], tP[v]);
    if (tP[v].isInRange(1, m_surface.getWidth() - 1, 0,
                        m_surface.getHeight())) // one pixel is added at the
                                                // left and right of every line
                                                // to fill gaps between
                                                // triangles.
    {
      isNotDrawn = false;
    } else {
      isFullyDrawn = false;
    }
  }

  if (isNotDrawn) // All points are Out-Of-View
    return;

  sortInPlace(tP);

  int x1, x2;
  float a1, a2;
  int yP;

  if (tP[2].y > 0) {
    yP = 0;
    if (tP[0].y == tP[2].y && tP[0].y > 0 &&
        tP[0].y < m_surface.getHeight() - 1) {
      drawLine(tP[0].x, tP[2].x, tP[0].y, color, isWireFrame);
    } else {
      a1 = (tP[0].x - tP[2].x) / ((float)tP[0].y - tP[2].y);
      a2 = (tP[1].x - tP[2].x) / ((float)tP[1].y - tP[2].y);
      for (int y = tP[2].y; y > tP[1].y; y--) {
        x1 = (int)(-a1 * yP);
        x2 = (int)(-a2 * yP);

        if (isFullyDrawn)
          drawLineNoBoundCheck(x1 + tP[2].x, x2 + tP[2].x, y, color,
                               isWireFrame);
        else
          drawLine(x1 + tP[2].x, x2 + tP[2].x, y, color, isWireFrame);

        yP++;
      }
    }
  } else // if this point's height is < 0, then it will be the same for all the
         // others
  {
    return;
  }

  if (tP[1].y > 0) {
    yP = 0;
    if (tP[0].y == tP[1].y && tP[0].y > 0 &&
        tP[0].y < m_surface.getHeight() - 1) {
      drawLine(tP[0].x, tP[1].x, tP[0].y, color, isWireFrame);
    } else {
      a1 = (tP[1].x - tP[0].x) / ((float)tP[1].y - tP[0].y);
      a2 = (tP[2].x - tP[0].x) / ((float)tP[2].y - tP[0].y);

      for (int y = tP[0].y; y <= tP[1].y; y++) {
        x1 = (int)(a1 * yP);
        x2 = (int)(a2 * yP);

        if (isFullyDrawn)
          drawLineNoBoundCheck(x1 + tP[0].x, x2 + tP[0].x, y, color,
                               isWireFrame);
        else
          drawLine(x1 + tP[0].x, x2 + tP[0].x, y, color, isWireFrame);

        yP++;
      }
    }
  }
}