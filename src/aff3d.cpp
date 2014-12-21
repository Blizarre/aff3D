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

#include <math.h>
#include <iostream>
#include <stdlib.h>
#include "SDLWrapper.h"
#include "SurfaceWrapper.h"

#include<vector>
#include <algorithm>

#include "vertex.h"
#include "triangle.h"
#include "transfo.h"
#include "IO.h"
#include "utils.h"
#include "stdlib.h"

using namespace std;

const Uint32 screenWidth = 640;
const Uint32 screenHeight = 640;

typedef struct _Point {
    int x, y;
    float z;
} Point;


/**
 * Projète le vertex dans l'espca de l'écran. le +0.5 permet de mettre le point de centre (x=0, y=0) au centre de l'écran 
 qui a par convention un champ de vue de 1 unité en X et 1 unité en Y.
 Pour Z, +2 pour être certain qu'on sera devant.
 **/
void projeter(const Vertex & i, Point & pt) {
    pt.x = (int)((i.x + 0.5) * screenWidth);
    pt.y = (int)((i.y + 0.5) * screenHeight);
    pt.z = (float)(i.z + 2.0);
}


void sortInPlace(Point pt[]) {
    Point tmp;
    if(pt[1].y < pt[0].y) {
        tmp = pt[0];
        pt[0] = pt[1];
        pt[1] = tmp;
    }

    if(pt[2].y < pt[1].y) {
        tmp = pt[1];
        pt[1] = pt[2];
        pt[2] = tmp;
    }

    if(pt[1].y < pt[0].y) {
        tmp = pt[0];
        pt[0] = pt[1];
        pt[1] = tmp;
    }
}


void drawVertex(SurfaceWrapper & surface, const Triangle & t) {
    Point tP[3];
    for (int v=0; v<3;v++)
        projeter(t.points[v], tP[v]);

    sortInPlace(tP);

    std::array<Uint32, 3> color = { surface.getColor(255, 255, 0), surface.getColor(255, 0, 255), surface.getColor(0, 255, 255) };

    for(int i=0; i< 3; i++) {
        surface.DrawPixel(tP[i].x - 1, tP[i].y, color[i]);
        surface.DrawPixel(tP[i].x + 1, tP[i].y, color[i]);
        surface.DrawPixel(tP[i].x, tP[i].y - 1, color[i]);
        surface.DrawPixel(tP[i].x, tP[i].y + 1, color[i]);
        surface.DrawPixel(tP[i].x, tP[i].y, color[i]);
    }
}

inline void sortAndTrim(int & val1, int & val2)
{
    if (val2 < val1)
    {
        int tmp = val1;
        val1 = val2;
        val2 = tmp;
    }

    val1--;
    val2++;

    if (val1 < 0)
        val1 = 0;
    else if (val1 >= screenWidth)
        val1 = screenWidth - 1;

    if (val2 < 0)
        val2 = 0;
    else if (val2 >= screenWidth)
        val2 = screenWidth - 1;
}

bool isInRangeY(int y) { return y > 0 && y < screenHeight - 1; }
bool isInRangeX(int x) { return x > 0 && x < screenWidth - 1; }

void drawLine(SurfaceWrapper & surface, int x1, int x2, int y, Uint32 color, bool isWireFrame)
{
    if (isInRangeY(y))
    {
        if (isWireFrame)
        {
            surface.DrawPixel(x1, y, color);
            surface.DrawPixel(x2, y, color);
        }
        else
        {
            sortAndTrim(x1, x2);
            for (int x = x1; x < x2; x++) // +1 et -1 pour éviter les artefacts "fil de fer"
                surface.DrawPixel(x, y, color);
        }
    }
}


/**
 * A triangle is draw in several steps: 
 * - first, the Vertex are projected on the screen space.
 * - then, they are ordered by height.
 * - the first half of the triangle is drawn (top point to the middle point)
 * - and finally the bottom part of the triangle is drawn
 **/
void drawTriangle(SurfaceWrapper & surface, const Triangle & t, bool isWireFrame) {
    // TODO: bring it in Triangle or on a specialized drawing class, code reuse between the two halves
    Vertex lightPoint(0.5574f,0.5574f, 0.5574f);
    float lightCoeff = lightPoint.x * t.points[3].x + lightPoint.y * t.points[3].y + lightPoint.z * t.points[3].z;
    lightCoeff = (lightCoeff > 0 ? lightCoeff : 0);

    Uint32 color = surface.getColor(static_cast<Uint8>(t.r * lightCoeff), static_cast<Uint8>(t.g * lightCoeff), static_cast<Uint8>(t.b * lightCoeff));

    Point tP[3];
    for (int v=0; v<3;v++)
        projeter(t.points[v], tP[v]);

    sortInPlace(tP);

    int x1, x2;
    float a1, a2;
    int yP;


    if (tP[2].y > 0)
    {
        yP = 0;
        if (tP[0].y == tP[2].y && tP[0].y > 0 && tP[0].y < screenHeight - 1) {
            drawLine(surface, tP[0].x, tP[2].x, tP[0].y, color, isWireFrame);
        }
        else {
            a1 = (tP[0].x - tP[2].x) / ((float)tP[0].y - tP[2].y);
            a2 = (tP[1].x - tP[2].x) / ((float)tP[1].y - tP[2].y);
            for (int y = tP[2].y; y > tP[1].y; y--) {
                x1 = (int)(-a1 * yP);
                x2 = (int)(-a2 * yP);

                drawLine(surface, x1 + tP[2].x, x2 + tP[2].x, y, color, isWireFrame);
                yP++;
            }
        }
    }
    else // if this point's height is < 0, then it will be the same for all the others
    {
        return;
    }

    if (tP[1].y > 0)
    {
        yP = 0;
        if (tP[0].y == tP[1].y && tP[0].y > 0 && tP[0].y < screenHeight - 1) {
            drawLine(surface, tP[0].x, tP[1].x, tP[0].y, color, isWireFrame);
        }
        else {
            a1 = (tP[1].x - tP[0].x) / ((float)tP[1].y - tP[0].y);
            a2 = (tP[2].x - tP[0].x) / ((float)tP[2].y - tP[0].y);


            for (int y = tP[0].y; y <= tP[1].y; y++) {
                x1 = (int)(a1 * yP);
                x2 = (int)(a2 * yP);

                drawLine(surface, x1 + tP[0].x, x2 + tP[0].x, y, color, isWireFrame);
                yP++;
            }
        }
    }


}



void flouterEcran(SurfaceWrapper & surface, std::array<signed char, 100> tabRandom) {
    int x, y, color;
    SurfaceWrapper temporary = surface; // copie de la surface pour éviter de propager les diffusions

    for (x = 10; x<screenWidth - 10; x++)
    for (y = 10; y < screenHeight - 10; y++) {
        color = temporary.GetPixel(x + tabRandom[x*y % 100], y + tabRandom[x*y % 100]);
        surface.DrawPixel(x, y, color);
    }
}

bool trierTriangle(const Triangle& d1, const Triangle& d2)
{
    return d1.sumOfDistances() < d2.sumOfDistances();
}

int main(int argc, char *argv[])
{
    vector<Triangle> vectTriangle;
    Transformation transfo;
    float delta[3];

    bool shouldQuit = false;
    unsigned int t=0, initTime = 0;
    int sleep;
    bool scrambleImage = false, isWireframe = false, backfaceC = false;
    vector<Triangle>::iterator it;

    bool autoAnimate = true;
	bool benchmarkMode = false;

    delta[0] = 0;
    delta[1] = 0;
    delta[2] = 0;

    int drawnTriangleCount = 0, frameCount = 0;
    if(argc < 2) {
        cout <<"Usage : " <<argv[0] <<" Modele /benchmark" <<endl;
        return EXIT_FAILURE;
    } 

	try
	{
		readFromFile(argv[1], vectTriangle);
	}
	catch (exception& e)
	{
		cerr << "Couldn't read data from file" << endl;
		cerr << "Message: " << e.what();
		return EXIT_FAILURE;
	}

	if (argc >= 3 && std::string("/benchmark") == argv[2])
	{
		benchmarkMode = true;
	}

    SDLWrapper sdl(screenWidth, screenHeight);
    SurfaceWrapper& screen = sdl.getMainScreen();

    initTime = sdl.getTicks();

    // this array of 100 pseudo-random values is used to speed up computations when the quality of the randomness
    // is not important. no need to call rand every time.
    // TODO: extract to his own object
    std::array<signed char, 100> tab;
    for (auto & val: tab)
        val=rand()%19-9; // between -9 and 9

    float rotX = 0.01f, rotY=0.01f;

    sdl.onMouseMotion([&rotX, &rotY](size_t x, size_t y) { rotX = y / 100.0f; rotY = x / 100.0f; });
    sdl.onQuitEvent([&shouldQuit]() {shouldQuit = true; } );
    sdl.onKeyPress(SDLK_ESCAPE, [&shouldQuit]() { shouldQuit = !shouldQuit; });
    sdl.onKeyPress(SDLK_f, [&scrambleImage]() { scrambleImage = !scrambleImage; });
    sdl.onKeyPress(SDLK_w, [&isWireframe]() { 
        isWireframe = !isWireframe;
    });
    sdl.onKeyPress(SDLK_b, [&backfaceC]() { 
        backfaceC = !backfaceC; 
    });
    sdl.onKeyPress(SDLK_q, [&autoAnimate]() { 
        autoAnimate = !autoAnimate;
    });

    while(!shouldQuit) { 
		if (benchmarkMode && frameCount >= 2000) break;

		Uint32 currentTime = sdl.getTicks();
        //cout << currentTime - t <<"ms (" << drawnTriangleCount << ")"<<endl;
        drawnTriangleCount = 0;
		if (!benchmarkMode)
		{
			sleep = 24 - (currentTime - t);
			SDL_Delay((sleep > 0 ? sleep : 1));
		}
		t = sdl.getTicks();

        screen.fill(50, 50, 50);

        //     delta[0] =   0; //cos(t/600.0)/4.0;
        //     delta[1] = 0.2; //sin(t/600.0)/3.0;
        transfo = Transformation();
        if(autoAnimate) {
            //      transfo.translate(delta);
            transfo.rotationX(t/6000.0f);
            transfo.rotationZ(t/50000.0f);
        } else {
            transfo.rotationX(rotX);
            transfo.rotationZ(rotY);
        }

		for (Triangle& tr : vectTriangle)
		{
			tr.applyTransformation(transfo);
		}

        std::sort(vectTriangle.begin(), vectTriangle.end(), trierTriangle );
        for(Triangle& tr : vectTriangle) {
            if(backfaceC || tr.isFacingCamera()) { 
                drawnTriangleCount ++;
                drawTriangle(screen, tr, isWireframe);
            }
        }

        if (scrambleImage)
            flouterEcran(screen, tab);

        sdl.flipBuffer();

		// Ignore all input during benchmark... Should be kept short !
        if (!benchmarkMode)
            sdl.processEvents();

		frameCount++;

    }   // End Game Loop
    cout <<frameCount <<" frames in "<<(sdl.getTicks()-initTime)<<" ms., mean fps : "<< int(frameCount / ((sdl.getTicks()-initTime)/1000.0)) <<endl;
    
	return 0;
}
