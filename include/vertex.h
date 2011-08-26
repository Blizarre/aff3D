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


#ifndef __VERTEX_V__1325465

#define __VERTEX_V__1325465

#include "transfo.h"
#include <cmath>

class Vertex {
    public:
    float x, y, z;
    
    /**
     * Constructeur par défaut : Attention, les valeurs de x, y et z ne sont pas initialisées
     **/
    Vertex() { }

    Vertex(const float _x, const float _y, const float _z): x(_x), y(_y), z(_z) { }
    Vertex(const Vertex &v): x(v.x), y(v.y), z(v.z) { }

    Vertex operator*(const Vertex v);
    Vertex operator-(const Vertex v);
    Vertex operator/(const float f);

    
    Vertex appliquerTransfo(const Transformation t);
    
    float norme();
    void normer();
};


#endif
