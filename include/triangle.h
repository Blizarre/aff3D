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

#ifndef __TRIANGLE_H_443354423

#define __TRIANGLE_H_443354423

#include "transfo.h"
#include "vertex.h"
#include <array>
#include "Normal.hpp"

typedef unsigned char u8;

//TODO: refactor this Triangle class to not expose the points directly
//and have a clear difference between points and normal

class Triangle {
public:
  // point 0,1 and 2 are the vertexes of the triangle, while 4 is the normal
  std::array<Vertex, 3> points;

  // original data points, without transformation
  std::array<Vertex, 3> origPoints;

  Normal normal;
  u8 r, g, b;

  Triangle() : r(0), g(0), b(0) {}

  Triangle(Vertex a, Vertex b, Vertex c, Normal n) {
    origPoints[0] = a;
    origPoints[1] = b;
    origPoints[2] = c;
    origNormal = n;
    normal = origNormal;
    points = origPoints; // copy the rawData to initialize points
    this->r = 0;
    this->g = 0;
    this->b = 255;
  }

  Triangle(Vertex a, Vertex b, Vertex c) {
    origPoints[0] = a;
    origPoints[1] = b;
    origPoints[2] = c;
    origNormal = Normal{ a, b, c };
    origNormal.normInPlace();

    points = origPoints; // copy the rawData to initialize points
    normal = origNormal;
    this->r = 0;
    this->g = 0;
    this->b = 255;
  }

  Triangle(Vertex a, Vertex b, Vertex c, Normal n, u8 cr, u8 cg, u8 cb) {
    origPoints[0] = a;
    origPoints[1] = b;
    origPoints[2] = c;
    origNormal = n;
    points = origPoints; // copy the rawData to initialize points
    normal = origNormal;
    this->r = cr;
    this->g = cg;
    this->b = cb;
  }

  void applyTransformation(const Transformation &tr);
  float sumOfDistances() const;
  bool isFacingCamera() const;

protected:
  Normal origNormal;
};

#endif
