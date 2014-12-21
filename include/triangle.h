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

#include <array>
#include "transfo.h"
#include "vertex.h"

typedef unsigned char u8;

class Triangle {
    public:
    u8 r, g, b;
	std::array<Vertex, 4> points;

	Triangle(): r(0), g(0), b(0) {} 

    Triangle(Vertex a, Vertex b, Vertex c, Vertex n) {
        m_rawData[0] = a;
        m_rawData[1] = b;
        m_rawData[2] = c;
        m_rawData[3] = n;
        points = m_rawData; // copy the rawData to initialize points
        this->r = 0;
        this->g = 0;
        this->b = 255;    
    }
    
    Triangle(Vertex a, Vertex b, Vertex c, Vertex n, u8 cr, u8 cg, u8 cb) {
        m_rawData[0] = a;
        m_rawData[1] = b;
        m_rawData[2] = c;
        m_rawData[3] = n;
        points = m_rawData; // copy the rawData to initialize points
        this->r = cr;
        this->g = cg;
        this->b = cb;    
    }

	void applyTransformation(const Transformation & tr);
    float sumOfDistances() const;
	bool Triangle::isFacingCamera() const;
    void setRawData(const std::array<Vertex, 4> & newRawData);

protected:
	// original data points, without transformation
	std::array<Vertex, 4> m_rawData; 

};

#endif
