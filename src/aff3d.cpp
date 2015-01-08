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
#include "Rasterizer.h"

#include <vector>
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


void flouterEcran(SurfaceWrapper & surface, std::array<signed char, 100> tabRandom) {
    int x, y;
    SurfaceWrapper temporary = surface; // copie de la surface pour éviter de propager les diffusions

    for (x = 10; x<screenWidth - 10; x++)
		for (y = 10; y < screenHeight - 10; y++) {
			surface.pixel(x, y) = temporary.pixel(x + tabRandom[x*y % 100], y + tabRandom[x*y % 100]);
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
	Rasterizer rasterizer(screen);
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
		
		screen.lockSurface();

		for(Triangle& tr : vectTriangle) {
            if(backfaceC || tr.isFacingCamera()) { 
                drawnTriangleCount ++;
                rasterizer.drawTriangle(tr, isWireframe);
            }
        }

        if (scrambleImage)
            flouterEcran(screen, tab);
		screen.unLockSurface();

        sdl.flipBuffer();

		// Ignore all input during benchmark... Should be kept short !
        if (!benchmarkMode)
            sdl.processEvents();

		frameCount++;

    }   // End Game Loop
    cout <<frameCount <<" frames in "<<(sdl.getTicks()-initTime)<<" ms., mean fps : "<< int(frameCount / ((sdl.getTicks()-initTime)/1000.0)) <<endl;
    
	return 0;
}
