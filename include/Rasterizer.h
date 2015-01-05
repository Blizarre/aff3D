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
#include "Vertex.h"
#include "Triangle.h"

typedef struct _Point {
	int x, y;
} Point;


/*
	Represent a Rasterizer object that draw triangles on a Surface. Since I didn't wanted to use [smart-]pointers
	as a challenger, the Rasterizer holds a "weak" reference to the SurfaceWrapper.
*/
class Rasterizer
{
public:
	
	// Create a new Rasterizer from the surface. Holds a weak reference to this surface. Do not let it out of scope !
	Rasterizer(SurfaceWrapper & surface) : m_surface(surface) { }

	// Project the vertex in the screen space. the view is centered around 0 in x and y thanks to the +0.5.
	inline void projectToScreen(const Vertex & i, Point & pt) {
		pt.x = (int)((i.x + 0.5) * m_surface.getWidth());
		pt.y = (int)((i.y + 0.5) * m_surface.getHeight());
	}


	// Sort three points along their y component
	void sortInPlace(Point pt[]) {
		Point tmp;
		if (pt[1].y < pt[0].y) {
			tmp = pt[0];
			pt[0] = pt[1];
			pt[1] = tmp;
		}

		if (pt[2].y < pt[1].y) {
			tmp = pt[1];
			pt[1] = pt[2];
			pt[2] = tmp;
		}

		if (pt[1].y < pt[0].y) {
			tmp = pt[0];
			pt[0] = pt[1];
			pt[1] = tmp;
		}
	}



	// Draw a line on the Surface. x1 and x2 can be both outside the screen range 
	inline void drawLine(int x1, int x2, int y, Uint32 color, bool isWireFrame);

	// Draw a triangle on the screen
	void drawTriangle(const Triangle & t, bool isWireFrame);

protected:

	SurfaceWrapper& m_surface;

	// sortAndTrimXValues will make sure that val1 < val2 (swap them if needed) and that they will be in the screen space
	inline void sortAndTrimXValues(int & val1, int & val2);

	bool isInRangeY(int y) { return y > 0 && y < m_surface.getHeight() - 1; }
	bool isInRangeX(int x) { return x > 0 && x < m_surface.getWidth() - 1; }

	// we simply "Steal" the reference of the other Rasterizer
	Rasterizer(const Rasterizer & other) : m_surface(other.m_surface) { }
	
	Rasterizer& operator=(const Rasterizer & other)
	{
		m_surface = other.m_surface;
	}
};

