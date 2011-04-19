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

#define SCR_X 512
#define SCR_Y 512


void DrawPixel(SDL_Surface *screen, int x, int y,Uint32 color)
{
    Uint32 *bufp;

    bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
    *bufp = color;
}

typedef struct _Point {
    int x, y;
    float z;
} Point;


void projeter(Vertex i, Point & pt) {
    pt.x = (int)((i.x + 1.0) * SCR_X/2.0);
    pt.y = (int)((i.y + 1.0) * SCR_Y/2.0); 
    pt.z = (float)((i.z) + 1.0);
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




void afficherVertex(SDL_Surface *screen, Triangle t) {
    Point tP[3];
    for (int v=0; v<3;v++)
        projeter(t.newState[v], tP[v]);
    
    trierInPlace(tP);
    
    Uint32 color[] = { SDL_MapRGB(screen->format, 255, 255, 0), SDL_MapRGB(screen->format, 255, 0, 255), SDL_MapRGB(screen->format, 0, 255, 255)};
    
    for(int i=0; i< 3; i++) {
        DrawPixel(screen, tP[i].x-1, tP[i].y, color[i]);
        DrawPixel(screen, tP[i].x+1, tP[i].y, color[i]);
        DrawPixel(screen, tP[i].x, tP[i].y-1, color[i]);
        DrawPixel(screen, tP[i].x, tP[i].y+1, color[i]);
        DrawPixel(screen, tP[i].x, tP[i].y, color[i]);        
    }
}


void dessinerLigne(SDL_Surface *screen, int xMin, int xMax, int y, Uint32 color) {
    
    for(int x = MAX(0, xMin); x < MIN(xMax, SCR_X); x++)
        DrawPixel(screen, x, y, color);
}

void dessinerLigne(SDL_Surface *screen, int xMin, int xMax, float zMin, float zMax, int y, float * zbuff, Uint32 color) {
    float nZ;
    float coeff = (zMax - zMin) / (xMax - xMin);
    for(int x = MAX(0, xMin); x < MIN(SCR_X - 1,  xMax); x++) {
        nZ = coeff * (x - xMin) + zMin;
    if(nZ < zbuff[x + SCR_X*y]) {
        DrawPixel(screen, x, y, color);
            zbuff[x + SCR_X*y] = nZ;
        }
    }
}



void afficherTriangle(SDL_Surface *screen, Triangle t, float * zbuff) {

    Uint32 color = SDL_MapRGB(screen->format, t.r, t.g, t.b);

    Point tP[3];
    for (int v=0; v<3;v++)
        projeter(t.newState[v], tP[v]);

    trierInPlace(tP);
        
    int x1, x2;
    float a1, a2;

    int yP = 0;
    if(tP[0].y == tP[1].y) {
        if(tP[0].x < tP[1].x)
            dessinerLigne(screen, tP[0].x, tP[1].x, tP[0].z, tP[1].z, tP[0].y, zbuff, color);
        else
            dessinerLigne(screen, tP[1].x, tP[0].x, tP[1].z, tP[0].z, tP[0].y, zbuff, color);
    } else {
        a1 = (tP[1].x - tP[0].x)/((float)tP[1].y - tP[0].y);
        a2 = (tP[2].x - tP[0].x)/((float)tP[2].y - tP[0].y);
        

        for(int y=MAX(0,tP[0].y); y<=MIN(tP[1].y,SCR_X-1); y++) {
            x1 = (int)(a1 * yP);
            x2 = (int)(a2 * yP);
            
            if(x1 < x2 )  dessinerLigne(screen, x1 + tP[0].x, x2 + tP[0].x, tP[0].z, tP[1].z, y, zbuff, color);
            else          dessinerLigne(screen, x2 + tP[0].x, x1 + tP[0].x, tP[1].z, tP[0].z, y, zbuff, color);            
            
            yP ++;
        }
    }

############ IL FAUT ADAPTER LES ZZZ !!!
    yP = 0; 
    if(tP[0].y == tP[2].y) {
        if(tP[0].x < tP[2].x)
            dessinerLigne(screen, tP[0].x, tP[2].x, tP[0].z, tP[2].z, tP[0].y, zbuff, color);
        else
            dessinerLigne(screen, tP[2].x, tP[0].x, tP[2].z, tP[0].z, tP[0].y, zbuff, color);
    } else {
        a1 = (tP[0].x - tP[2].x)/((float)tP[0].y - tP[2].y);
        a2 = (tP[1].x - tP[2].x)/((float)tP[1].y - tP[2].y);
        for(int y = MIN(SCR_X-1, tP[2].y); y > MAX(0, tP[1].y); y--) {
            x1 = (int)(- a1 * yP);
            x2 = (int)(- a2 * yP);

            if( x1 < x2 ) dessinerLigne(screen, x1 + tP[2].x, x2 + tP[2].x, tP[2].z, tP[1].z, y, zbuff, color);
            else          dessinerLigne(screen, x2 + tP[2].x, x1 + tP[2].x, tP[1].z, tP[2].z, y, zbuff, color);            

            yP ++;
        }
    }
    
 
}


int main(int argc, char *argv[])
{
 cout <<"Initializing SDL." << endl;
  
 if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)< 0) {
    cout <<"Could not initialize SDL:" << SDL_GetError() << endl;
    SDL_Quit();
  } else {
      cout << "Audio & Video initialized correctly" << endl;
  }
  
  vector<Triangle> vectTriangle;
  readFromFile("test.txt", vectTriangle);
  
  SDL_Surface *screen;

  screen = SDL_SetVideoMode(SCR_X, SCR_Y, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
  if ( screen == NULL ) {
      cerr << "Impossible de passer en "<<SCR_X << "x" << SCR_Y <<" : " << SDL_GetError() <<endl;
      SDL_Quit();
  }

  Transformation transfo;
  float delta[3];
  delta[0] = 0;
  delta[1] = 0;
  delta[2] = 0;

  float zbuff[SCR_X*SCR_Y];

  SDL_Event event;
  int done = 0;
  unsigned int t=0;
  int sleep;

  vector<Triangle>::iterator it;

  while(!done) { //  Game Loop
      cout <<SDL_GetTicks() - t <<"ms" <<endl;
      sleep = 24 - (SDL_GetTicks() - t);
      SDL_Delay( (sleep > 0?sleep:1) );
      t = SDL_GetTicks();
      SDL_FillRect( screen, NULL, SDL_MapRGB(screen->format, 50, 50, 50));

      for(int i=0;i<SCR_X*SCR_Y;i++)
          zbuff[i]=1.0;

      delta[0] = cos(t/600.0)/4.0;
      delta[1] = sin(t/600.0)/3.0;
      transfo = Transformation();
 //     transfo.translate(delta);
 //     transfo.rotationZ(t/6000.0);
      transfo.rotationX(t/3000.0);
 
      for(it = vectTriangle.begin(); it != vectTriangle.end(); it++) {
          it->appliquerTransfo(transfo);
          afficherTriangle(screen, (*it), zbuff);
          afficherVertex(screen, (*it));
      }
      float maxZB = 0, minZB=255;
        for(int x=0;x<SCR_X;x++)
          for(int y=0;y<SCR_Y;y++) {
            if(zbuff[x + SCR_X*y] == 1.0) zbuff[x + SCR_X*y] = 0;
            if(zbuff[x + SCR_X*y] > maxZB) maxZB=zbuff[x + SCR_X*y];
            if(zbuff[x + SCR_X*y] < minZB) minZB=zbuff[x + SCR_X*y];

            DrawPixel(screen, x, y, SDL_MapRGB(screen->format, (int)((zbuff[x + SCR_X*y]) * 250), 10, 10));
          }
      cout << "max : " << maxZB << endl;
      cout << "min : " << minZB << endl;
      SDL_Flip(screen);

      if(SDL_PollEvent(&event)) switch(event.type) {

          case SDL_KEYDOWN:
               done = true;
              break;

          case SDL_QUIT:
              done = true;
              break;
      }
  }   // End Game Loop


  SDL_Quit();
}
