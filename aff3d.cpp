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
#include <SDL_endian.h> /* Used for the endian-dependent 24 bpp mode */


#include "vertex.h"
#include "triangle.h"
#include "transfo.h"

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
    pt.z = i.z * 320;
}

void afficherVertex(SDL_Surface *screen, Vertex lVertex[], int tVertex) {
    Point proj;

    Uint32 color = SDL_MapRGB(screen->format, 255, 255, 255);
    
    for(int i=0; i< tVertex; i++) {
        projeter(lVertex[i], proj);
        DrawPixel(screen, proj.x-1, proj.y, color);
        DrawPixel(screen, proj.x+1, proj.y, color);
        DrawPixel(screen, proj.x, proj.y-1, color);
        DrawPixel(screen, proj.x, proj.y+1, color);
        DrawPixel(screen, proj.x, proj.y, color);        
        
    }
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



void dessinerLigne(SDL_Surface *screen, int xMin, int xMax, int y, Uint32 color) {
    for(int x = xMin; x < xMax; x++)
        DrawPixel(screen, x, y, color);
}

#define MIN(a,b) (a<b?a:b)
#define MAX(a,b) (a>b?a:b)

void afficherTriangle(SDL_Surface *screen, Triangle t) {

    Uint32 colorFond = SDL_MapRGB(screen->format, 255, 50, 60);
    Uint32 colorLigneGauche = SDL_MapRGB(screen->format, 0, 0, 255);
    Uint32 colorLigneDroite = SDL_MapRGB(screen->format, 0, 255, 255);

    float zbuff[640*640];

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
            dessinerLigne(screen, tP[0].x, tP[1].x, tP[0].y, colorFond);
        else
            dessinerLigne(screen, tP[1].x, tP[0].x, tP[0].y, colorFond);
    } else {
        a1 = (vG.x - tP[0].x)/((float)vG.y - tP[0].y);
        a2 = (vD.x - tP[0].x)/((float)vD.y - tP[0].y);
        for(int y=tP[0].y; y<=tP[1].y; y++) {
            xMin = (int)(a1 * yP);
            xMax = (int)(a2 * yP);

            dessinerLigne(screen, xMin + tP[0].x, xMax + tP[0].x, y, colorFond);
            DrawPixel(screen, xMin + tP[0].x, y, colorLigneGauche);
            DrawPixel(screen, xMax + tP[0].x, y, colorLigneDroite);
            
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
            dessinerLigne(screen, tP[0].x, tP[2].x, tP[0].y, colorFond);
        else
            dessinerLigne(screen, tP[2].x, tP[0].x, tP[0].y, colorFond);
    } else {
        a1 = (vG.x - tP[2].x)/((float)vG.y - tP[2].y);
        a2 = (vD.x - tP[2].x)/((float)vD.y - tP[2].y);
        for(int y=tP[2].y; y>tP[1].y; y--) {
            xMin = (int)(-a1 * yP);
            xMax = (int)(-a2 * yP);
            
            dessinerLigne(screen, xMin + tP[2].x , xMax + tP[2].x, y, colorFond);
            DrawPixel(screen, xMin + tP[2].x, y, colorLigneGauche);
            DrawPixel(screen, xMax + tP[2].x, y, colorLigneDroite);

            yP ++;
        }
    }
    
    afficherVertex(screen, t.newState, 3);
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
  
  SDL_Surface *screen;

  screen = SDL_SetVideoMode(640, 640, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
  if ( screen == NULL ) {
      cerr << "Impossible de passer en 640x640 : " << SDL_GetError() <<endl;
      SDL_Quit();
  }

  Triangle t1 = Triangle( Vertex(0.0, 0.0, 0.0), Vertex(0.5, 0.0, 0.1), Vertex(0.0, 0.5, 0.0) );
  Triangle t2 = Triangle( Vertex(0.0, 0.0, 0.0), Vertex(-0.5, 0.0, 0.0), Vertex(0.0, -0.5, 0.0) );
  Triangle t3 = Triangle( Vertex(0.1, - 0.1, 0.1), Vertex(0.4, - 0.1, 0.1),    Vertex(0.1, -0.4, 0.1)    );
  Triangle t4 = Triangle( Vertex(- 0.1,  0.1, -0.1), Vertex(- 0.4, 0.1,- 0.1),    Vertex(- 0.1, 0.4, -0.1)    );

  Transformation transfo;
  float delta[3];
  delta[0] = 0;
  delta[1] = 0;
  delta[2] = 0;

  SDL_Event event;
  int done = 0;
  unsigned int t=0;
  int sleep;
  while((!done)) {
      cout <<SDL_GetTicks() - t <<"ms" <<endl;
      sleep = 24 - (SDL_GetTicks() - t);
      SDL_Delay( (sleep > 0?sleep:1) );
      t = SDL_GetTicks();
      SDL_FillRect( screen, NULL, SDL_MapRGB(screen->format, 50, 50, 50));
     
      delta[0] = cos(t/600.0)/4.0;
      delta[1] = sin(t/600.0)/3.0;
      transfo = Transformation();
      transfo.translate(delta);
      transfo.rotationZ(t/6000.0);
      transfo.rotationX(t/3000.0);

      t1.appliquerTransfo(transfo);
      t2.appliquerTransfo(transfo);
      t3.appliquerTransfo(transfo);
      t4.appliquerTransfo(transfo);
      
      afficherTriangle(screen, t1);
      afficherTriangle(screen, t2);
      afficherTriangle(screen, t3);
      afficherTriangle(screen, t4);
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
