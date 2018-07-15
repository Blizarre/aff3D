/***************************************************************************
*   Copyright (C) 2010 by Marache-Francisco   *
*   marache@IV   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#pragma once

#include "SDLWrapper.h"
#include "normal.h"
#include "triangle.h"
#include "vertex.h"

// It is the projection of a Vertex. in the screen space.
struct Point {
  int x, y;

  inline bool isInRange(int minX, int maxX, int minY, int maxY) {
    return (x > minX && x < maxX) && (y > minY && y < maxY);
  }
};

/*
   This class will draw triangles on a Surface.
*/
class Rasterizer {
public:
  // Create a new Rasterizer from the surface. Holds a weak reference to this
  // surface.
  Rasterizer(SurfaceWrapper &surface) : m_surface(surface) {}

  // Project the vertex in the screen space. the view is centered around 0 in x
  // and y thanks to the +0.5.
  void projectToScreen(const Vertex &i, Point &pt) {
    pt.x = (int)((i.x + 0.5) * m_surface.getWidth());
    pt.y = (int)((i.y + 0.5) * m_surface.getHeight());
  }

  /*
  * Draw the horizontal line between the two points start and end at height y.
  * Make a
  * boundary check: draw only the visible part of the line, check start, end and
  * y
  * preconditions: start <= end
  */
  void drawLine(int start, int end, int y, Uint32 color, bool isWireFrame);

  // Draw a triangle on the screen
  void drawTriangle(const Triangle &t, Normal &lightSource, bool isWireFrame,
                    bool backFaceCulling);

protected:
  SurfaceWrapper &m_surface;

  // trimXValues will make sure that va1 and val2 are in the screen space.
  // Warning,
  // val1 < val2 must be true
  void trimXValues(int &val1, int &val2);

  bool isInRangeY(int y) { return y >= 0 && y < m_surface.getHeight(); }
  bool isInRangeX(int x) { return x >= 0 && x < m_surface.getWidth(); }
  bool isLineInRangeX(int start, int end) {
    return start < m_surface.getWidth() && end > 0;
  }
};
