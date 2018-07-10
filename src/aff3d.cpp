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

#include "SDLWrapper.h"
#include "rasterizer.h"
#include "surfaceWrapper.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include <algorithm>
#include <vector>

#include "IO/fileParser.h"
#include "IO/triangleNormaliser.h"
#include "transfo.h"
#include "triangle.h"
#include "vertex.h"
#include "chronometer.hpp"
#include "normal.h"
#include "stdlib.h"
#include "utils.h"

using namespace std;

const Uint32 screenWidth = 1024;
const Uint32 screenHeight = 1024;

vector<Triangle> readFromFile(const string &fileName) {
  auto parser = FileParser::getParser(fileName);
  std::vector<Triangle> triangles = parser->triangles();
  std::cout << "File description: " << parser->description() << std::endl;
  TriangleNormalizer normalizer(triangles.cbegin(), triangles.cend());
  return normalizer.normalize(triangles.cbegin(), triangles.cend());
}


bool compareTriangleZ(const Triangle &d1, const Triangle &d2) {
  return d1.sumOfDistances() < d2.sumOfDistances();
}

// unit tests have their own main.
#ifndef UNIT_TESTS
// TODO: refactor the function, create a wrapper class for the rendering loop
int main(int argc, char *argv[]) {
  vector<Triangle> vectTriangle;
  Transformation transfo;

  bool shouldQuit = false;
  unsigned int initTime = 0;
  bool isWireframe = false, backfaceC = false;

  bool autoAnimate = false;
  bool benchmarkMode = false;

  int drawnTriangleCount = 0, frameCount = 0;
  if (argc < 2) {
    cout << "Usage : " << argv[0] << " <stl file> [/benchmark]" << endl;
    return EXIT_FAILURE;
  }

  try {
    vectTriangle = readFromFile(argv[1]);
  } catch (ParseError &e) {
    cerr << "Couldn't read data from file" << endl;
    cerr << "Message: " << e.what();
    return EXIT_FAILURE;
  }

  if (argc >= 3 && std::string("/benchmark") == argv[2]) {
    benchmarkMode = true;
  }

  SDLWrapper sdl(screenWidth, screenHeight);
  SurfaceWrapper &screen = sdl.getMainScreen();
  Rasterizer rasterizer(screen);
  initTime = sdl.getTicks();

  Chronometer<unsigned int> chrWait{"Wait before drawing", sdl.getTicks},
    chrFillScreen{"Fill Screen", sdl.getTicks},
    chrTransform{"Transform triangles", sdl.getTicks},
    chrSort{"Sort triangles", sdl.getTicks},
    chrRaster{"Raster triangles", sdl.getTicks},
    chrPresentImage{"Present image", sdl.getTicks};

  // this array of 100 pseudo-random values is used to speed up computations
  // when the quality of the randomness
  // is not important. no need to call rand every time.
  // TODO: extract to his own object
  std::array<signed char, 100> tab;
  for (auto &val : tab)
    val = rand() % 19 - 9; // between -9 and 9

  float rotX = 0.00f, rotY = 0.00f;

  sdl.onMouseMotion([&rotX, &rotY](size_t x, size_t y) {
    rotX = y / 100.0f;
    rotY = x / 100.0f;
  });
  sdl.onQuitEvent([&shouldQuit]() { shouldQuit = true; });
  sdl.onKeyPress(SDLK_ESCAPE, [&shouldQuit]() { shouldQuit = !shouldQuit; });
  sdl.onKeyPress(SDLK_w, [&isWireframe]() { isWireframe = !isWireframe; });
  sdl.onKeyPress(SDLK_b, [&backfaceC]() { backfaceC = !backfaceC; });
  sdl.onKeyPress(SDLK_q, [&autoAnimate]() { autoAnimate = !autoAnimate; });

  Normal lightSource = Normal{ 0, 0, 1 };
  lightSource.normInPlace();

  Uint32 lastStartRenderStep = 0;

  while (!shouldQuit) {
    Uint32 startRenderStep = sdl.getTicks();

    if (benchmarkMode && frameCount >= 2000)
      break; // in benchmark mode, exit after 2000 frames

    drawnTriangleCount = 0;
    if (!benchmarkMode) {
      int sleep = 24 - (startRenderStep - lastStartRenderStep);
      SDL_Delay((sleep > 0 ? sleep : 1));
    }
    lastStartRenderStep = startRenderStep;

    chrWait.addTimeSince(startRenderStep);


    screen.fill(50, 50, 50);
    chrFillScreen.addTimeSince(chrWait.lastEndTime());

    transfo = Transformation();

    transfo.translate({ 0, 0, 2 });

    if (autoAnimate) {
      transfo.rotationX(startRenderStep/ 6000.0f);
      transfo.rotationZ(startRenderStep / 50000.0f);
    } else {
      transfo.rotationX(rotX);
      transfo.rotationZ(rotY);
    }

    for (Triangle &tr : vectTriangle) {
      tr.applyTransformation(transfo);
    }
    chrTransform.addTimeSince(chrFillScreen.lastEndTime());

    // Sort triangles by they distance from the camera. The triangles will be
    // drawn in the sorted order, from the
    // the back to the front.
    std::sort(vectTriangle.begin(), vectTriangle.end(), compareTriangleZ);
    chrSort.addTimeSince(chrTransform.lastEndTime());

    screen.lockSurface();

    for (Triangle &tr : vectTriangle) {
      // if backface Culling is activated, test the angle of the normal of the
      // triangle to check if it is facing the camera
      if (backfaceC || tr.isFacingCamera()) {
        drawnTriangleCount++;
        rasterizer.drawTriangle(tr, lightSource, isWireframe);
      }
    }
    chrRaster.addTimeSince(chrSort.lastEndTime());

    screen.unLockSurface();

    sdl.flipBuffer();

    chrPresentImage.addTimeSince(chrRaster.lastEndTime());

    // Ignore all input during benchmark... Should be kept short !
    if (!benchmarkMode) {
      sdl.processEvents();
    }

    frameCount++;

  } // End Game Loop
  cout << frameCount << " frames in " << (sdl.getTicks() - initTime)
       << " ms., mean fps : "
       << int(frameCount / ((sdl.getTicks() - initTime) / 1000.0)) << endl
       << chrWait << chrFillScreen << chrTransform << chrSort << chrRaster << chrPresentImage;

  return 0;
}
#endif
