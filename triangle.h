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

#include "vertex.h"
#include "transfo.h"

typedef unsigned char u8;

class Triangle {
    public:
    u8 r, g, b;
    Vertex points[4];
    Vertex newState[4];

    Triangle() {
        Triangle(Vertex(), Vertex(), Vertex(), Vertex());
    }
    
    Triangle(Vertex a, Vertex b, Vertex c, Vertex n){
        this->points[0] = a;
        this->points[1] = b;
        this->points[2] = c;
        this->points[3] = n;
        this->r = 0;
        this->g = 0;
        this->b = 255;    
    }
    
    Triangle(Vertex a, Vertex b, Vertex c, Vertex n, u8 cr, u8 cg, u8 cb){
        this->points[0] = a;
        this->points[1] = b;
        this->points[2] = c;
        this->points[3] = n;
        this->r = cr;
        this->g = cg;
        this->b = cb;    
    }

    void appliquerTransfo(Transformation t);

    float distanceMoyenne() const;
    bool estAffiche();
};

#endif
