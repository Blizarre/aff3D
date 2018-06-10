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

#include "stdlib.h"
#include "utils.h"

using namespace std;

const Uint32 screenWidth = 640;
const Uint32 screenHeight = 640;

vector<Triangle> readFromFile(const string &fileName) {
  auto parser = FileParser::getParser(fileName);
  std::vector<Triangle> triangles = parser->triangles();
  std::cout << "File description: " << parser->description() << std::endl;
  TriangleNormalizer normalizer(triangles.cbegin(), triangles.cend());
  return normalizer.normalize(triangles.cbegin(), triangles.cend());
}

void scrambleImage(SurfaceWrapper &surface,
                   std::array<signed char, 100> tabRandom) {
  Uint32 x, y;
  SurfaceWrapper temporary =
      surface;

  for (x = 10; x < screenWidth - 10; x++)
    for (y = 10; y < screenHeight - 10; y++) {
      surface.pixel(x, y) = temporary.pixel(x + tabRandom[x * y % 100],
                                            y + tabRandom[x * y % 100]);
    }
}

bool compareTriangleZ(const Triangle &d1, const Triangle &d2) {
  return d1.sumOfDistances() < d2.sumOfDistances();
}

// TODO: refactor the function, create a wrapper class for the rendering loop
int main(int argc, char *argv[]) {
  vector<Triangle> vectTriangle;
  Transformation transfo;

  bool shouldQuit = false;
  unsigned int initTime = 0;
  int sleep;
  bool scramble = false, isWireframe = false, backfaceC = false;

  bool autoAnimate = true;
  bool benchmarkMode = false;

  int drawnTriangleCount = 0, frameCount = 0;
  if (argc < 2) {
    cout << "Usage : " << argv[0] << " <stl file> [/benchmark]" << endl;
    return EXIT_FAILURE;
  }

  try {
    vectTriangle = readFromFile(argv[1]);
  } catch (runtime_error &e) {
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

  float rotX = 0.01f, rotY = 0.01f;

  sdl.onMouseMotion([&rotX, &rotY](size_t x, size_t y) {
    rotX = y / 100.0f;
    rotY = x / 100.0f;
  });
  sdl.onQuitEvent([&shouldQuit]() { shouldQuit = true; });
  sdl.onKeyPress(SDLK_ESCAPE, [&shouldQuit]() { shouldQuit = !shouldQuit; });
  sdl.onKeyPress(SDLK_f, [&scramble]() { scramble = !scramble; });
  sdl.onKeyPress(SDLK_w, [&isWireframe]() { isWireframe = !isWireframe; });
  sdl.onKeyPress(SDLK_b, [&backfaceC]() { backfaceC = !backfaceC; });
  sdl.onKeyPress(SDLK_q, [&autoAnimate]() { autoAnimate = !autoAnimate; });


  while (!shouldQuit) {
    Uint32 startRenderFrame = sdl.getTicks();

    if (benchmarkMode && frameCount >= 2000)
      break; // in benchmark mode, exit after 2000 frames

    drawnTriangleCount = 0;
    if (!benchmarkMode) {
      sleep = 24 - (sdl.getTicks() - startRenderFrame);
      SDL_Delay((sleep > 0 ? sleep : 1));
    }

    chrWait.addTimeSince(startRenderFrame);

    screen.fill(50, 50, 50);
    chrFillScreen.addTimeSince(chrWait.lastEndTime());

    transfo = Transformation();
    if (autoAnimate) {
      transfo.rotationX(startRenderFrame/ 6000.0f);
      transfo.rotationZ(startRenderFrame / 50000.0f);
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
        rasterizer.drawTriangle(tr, isWireframe);
      }
    }
    chrRaster.addTimeSince(chrSort.lastEndTime());

    if (scramble)
      scrambleImage(screen, tab);

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
