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

#include"vertex.h"


Vertex Vertex::appliquerTransfo(Transformation t) {
    float * coeffs = t.matrix;
    float ox, oy, oz;
    float nx, ny, nz;

    ox = this->x; oy = this->y; oz = this->z;
    nx = ox * coeffs[0] + oy * coeffs[1] + oz * coeffs[2]  + coeffs[3];
    ny = ox * coeffs[4] + oy * coeffs[5] + oz * coeffs[6]  + coeffs[7];
    nz = ox * coeffs[8] + oy * coeffs[9] + oz * coeffs[10] + coeffs[11];
    return Vertex(nx, ny, nz);
}


