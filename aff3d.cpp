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
#include "SDL.h"
#include<vector>

#include "vertex.h"
#include "triangle.h"
#include "transfo.h"
#include "IO.h"

using namespace std;



void DrawPixel(SDL_Surface *screen, int x, int y,Uint32 color)
{
    Uint32 *bufp;

    bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
    *bufp = color;
}

typedef struct _Point {
    int x, y, z;
} Point;


void projeter(Vertex i, Point & pt) {
    pt.x = (i.x + 1.0) * 320;
    pt.y = (i.y + 1.0) * 320; 
    pt.z = (i.z+1.0)*128;
}

void afficherVertex(SDL_Surface *screen, Vertex lVertex[], int tVertex) {
    Point proj;

//    Uint32 color = SDL_MapRGB(screen->format, 255, 255, 255);
//    
//    for(int i=0; i< tVertex; i++) {
//        projeter(lVertex[i], proj);
//        DrawPixel(screen, proj.x-1, proj.y, color);
//        DrawPixel(screen, proj.x+1, proj.y, color);
//        DrawPixel(screen, proj.x, proj.y-1, color);
//        DrawPixel(screen, proj.x, proj.y+1, color);
//        DrawPixel(screen, proj.x, proj.y, color);        
//        
//    }
}


void trierInPlace(Point pt[]) {
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

#define MIN(a,b) (a<b?a:b)
#define MAX(a,b) (a>b?a:b)


void dessinerLigne(SDL_Surface *screen, int xMin, int xMax, int y, Uint32 color) {
    
    for(int x = MAX(0, xMin); x < MIN(xMax, 640); x++)
        DrawPixel(screen, x, y, color);
}

void dessinerLigne(SDL_Surface *screen, int xMin, int xMax, float zMin, float zMax, int y, int * zbuff, Uint32 color) {
    float nZ;
    if(y < 0 || y > 639) return;
    float coeff = (zMax - zMin) / (xMax - xMin);
    for(int x = MAX(0, xMin+1); x < MIN(639, xMax-1); x++) {
        nZ = coeff * (x - xMin) + zMin;
        if(nZ < zbuff[x + 640*y]) {
            DrawPixel(screen, x, y, color);
            zbuff[x + 640*y] = nZ;
        }
    }
}



void afficherTriangle(SDL_Surface *screen, Triangle t, int * zbuff) {

    Uint32 color = SDL_MapRGB(screen->format, t.r, t.g, t.b);

    Point tP[3];
    for (int v=0; v<3;v++)
        projeter(t.newState[v], tP[v]);

    trierInPlace(tP);
        
    int xMin, xMax, yP = 0;
    float a1, a2;

    Point vG, vD;
    if(tP[1].x < tP[2].x) {
        vG = tP[1];
        vD = tP[2];
    } else {
        vD = tP[1];
        vG = tP[2];
    }

    if(tP[0].y == tP[1].y) {
        if(tP[0].x < tP[1].x)
            dessinerLigne(screen, tP[0].x, tP[1].x, tP[0].z, tP[1].z, tP[0].y, zbuff, color);
        else
            dessinerLigne(screen, tP[1].x, tP[0].x, tP[1].z, tP[0].z, tP[0].y, zbuff, color);
    } else {
        a1 = (vG.x - tP[0].x)/((float)vG.y - tP[0].y);
        a2 = (vD.x - tP[0].x)/((float)vD.y - tP[0].y);
        for(int y=tP[0].y; y<=tP[1].y; y++) {
            xMin = (int)(a1 * yP);
            xMax = (int)(a2 * yP);

            dessinerLigne(screen, xMin + tP[0].x, xMax + tP[0].x, vG.z, vD.z,  y, zbuff, color);
       
            yP ++;
        }
    }

    if(tP[0].x < tP[1].x) {
        vG = tP[0];
        vD = tP[1];
    } else {
        vD = tP[0];
        vG = tP[1];
    }

    yP = 0; 
    if(tP[0].y == tP[2].y) {
        if(tP[0].x < tP[2].x)
            dessinerLigne(screen, tP[0].x, tP[2].x, tP[0].z, tP[2].z, tP[0].y, zbuff, color);
        else
            dessinerLigne(screen, tP[2].x, tP[0].x, tP[2].z, tP[0].z, tP[0].y, zbuff, color);
    } else {
        a1 = (vG.x - tP[2].x)/((float)vG.y - tP[2].y);
        a2 = (vD.x - tP[2].x)/((float)vD.y - tP[2].y);
        for(int y=tP[2].y; y>tP[1].y; y--) {
            xMin = (int)(-a1 * yP);
            xMax = (int)(-a2 * yP);
            
            dessinerLigne(screen, xMin + tP[2].x , xMax + tP[2].x, vG.z, vD.z, y, zbuff,  color);

            yP ++;
        }
    }
}


int main(int argc, char *argv[])
{
 cout <<"Initializing SDL." << endl;
  /* Initializes Audio and the CDROM, add SDL_INIT_VIDEO for Video */
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)< 0) {
    cout <<"Could not initialize SDL:" << SDL_GetError() << endl;
    SDL_Quit();
  } else {
      cout << "Audio & Video initialized correctly" << endl;
  }
  
  vector<Triangle> vectTriangle;
  readFromFile("test.txt", vectTriangle);
  
  SDL_Surface *screen;

  screen = SDL_SetVideoMode(640, 640, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
  if ( screen == NULL ) {
      cerr << "Impossible de passer en 640x640 : " << SDL_GetError() <<endl;
      SDL_Quit();
  }

  Transformation transfo;
  float delta[3];
  delta[0] = 0;
  delta[1] = 0;
  delta[2] = 0;

  int zbuff[640*640];

  SDL_Event event;
  int done = 0;
  unsigned int t=0;
  int sleep;
  
  vector<Triangle>::iterator it;
  
  while((!done)) {
      cout <<SDL_GetTicks() - t <<"ms" <<endl;
      sleep = 24 - (SDL_GetTicks() - t);
      SDL_Delay( (sleep > 0?sleep:1) );
      t = SDL_GetTicks();
      SDL_FillRect( screen, NULL, SDL_MapRGB(screen->format, 50, 50, 50));
     
      for(int i=0;i<640*640;i++)
          zbuff[i]=255;

      delta[0] = cos(t/600.0)/4.0;
      delta[1] = sin(t/600.0)/3.0;
      transfo = Transformation();
 //     transfo.translate(delta);
 //     transfo.rotationZ(t/6000.0);
      transfo.rotationX(t/3000.0);
 
      for(it = vectTriangle.begin(); it != vectTriangle.end(); it++) {
          it->appliquerTransfo(transfo);
          afficherTriangle(screen, (*it), zbuff);
      }

      for(int x=0;x<640;x++)
          for(int y=0;y<640;y++)
              DrawPixel(screen, x, y, SDL_MapRGB(screen->format, zbuff[x + 640*y], 10, 10));
      SDL_Flip(screen);

      if(SDL_PollEvent(&event)) switch(event.type) {
                
          case SDL_KEYDOWN:
               done = 1;
              break;

          case SDL_QUIT:
              done = true;
              break;
      }   // End switch
            
  }   // End while
        

  SDL_Quit();
}
