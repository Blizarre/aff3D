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

#include "vertex.h"
#include "transfo.h"

class Triangle {
    public:
    Vertex points[3];
    Vertex newState[3];

    Triangle() {
        points[0] = Vertex();
        points[1] = Vertex();
        points[2] = Vertex();
    }

    Triangle(Vertex a, Vertex b, Vertex c){
        this->points[0] = a;
        this->points[1] = b;
        this->points[2] = c;
    }

    void appliquerTransfo(Transformation t);
};
