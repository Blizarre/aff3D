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
#include <algorithm>

#include "vertex.h"
#include "triangle.h"
#include "transfo.h"
#include "IO.h"
#include "utils.h"

using namespace std;

#define SCR_X 640
#define SCR_Y 640


void DrawPixel(SDL_Surface *screen, int x, int y,Uint32 color)
{
    Uint32 *bufp;

    bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
    *bufp = color;
}

Uint32 GetPixel(SDL_Surface *screen, int x, int y)
{
    Uint32 *bufp;

    bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
    return *bufp;
}

typedef struct _Point {
    int x, y;
    float z;
} Point;


/**
 * Projète le vertex dans l'espca de l'écran. le +0.5 permet de mettre le point de centre (x=0, y=0) au centre de l'écran 
 qui a par convention un champ de vue de 1 unité en X et 1 unité en Y.
 Pour Z, +2 pour être certain qu'on sera devant.
 **/
void projeter(Vertex i, Point & pt) {
    pt.x = (int)((i.x + 0.5)*SCR_X);
    pt.y = (int)((i.y + 0.5) * SCR_Y); 
    pt.z = (float)(i.z + 2.0);
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


void dessinerLigne(SDL_Surface *screen, int xMin, int xMax, int y, Uint32 color, bool isWireFrame) {
    if(isWireFrame and y > 0 and y < SCR_Y) {
        DrawPixel(screen, xMin, y, color);
        DrawPixel(screen, xMax, y, color);
    } else 
        for(int x = MAX(0, xMin-1); x < MIN(SCR_X - 1,  xMax+1); x++) // +1 et -1 pour éviter les artefacts "fil de fer"
            DrawPixel(screen, x, y, color);
}



void afficherTriangle(SDL_Surface *screen, Triangle t, bool isWireFrame) {

    Vertex lumiere(0.5574,0.5574, 0.5574);
    float light = lumiere.x * t.newState[3].x + lumiere.y * t.newState[3].y + lumiere.z * t.newState[3].z;
    light = (light > 0 ? light : 0);

    Uint32 color = SDL_MapRGB(screen->format, t.r * light , t.g * light, t.b * light);

    Point tP[3];
    for (int v=0; v<3;v++)
        projeter(t.newState[v], tP[v]);

    trierInPlace(tP);

    int x1, x2;
    float a1, a2;

    int yP = 0;
    if(tP[0].y == tP[1].y && tP[0].y > 0 && tP[0].y < SCR_Y-1) {
        if(tP[0].x < tP[1].x)
            dessinerLigne(screen, tP[0].x, tP[1].x, tP[0].y, color, isWireFrame);
        else
            dessinerLigne(screen, tP[1].x, tP[0].x, tP[0].y, color, isWireFrame);
    } else {
        a1 = (tP[1].x - tP[0].x)/((float)tP[1].y - tP[0].y);
        a2 = (tP[2].x - tP[0].x)/((float)tP[2].y - tP[0].y);


        for(int y=MAX(0,tP[0].y); y<=MIN(tP[1].y,SCR_X-1); y++) {
            x1 = (int)(a1 * yP);
            x2 = (int)(a2 * yP);

            if(x1 < x2 )  dessinerLigne(screen, x1 + tP[0].x, x2 + tP[0].x, y, color, isWireFrame);
            else          dessinerLigne(screen, x2 + tP[0].x, x1 + tP[0].x, y, color, isWireFrame);            

            yP ++;
        }
    }

    yP = 0; 
    if(tP[0].y == tP[2].y && tP[0].y > 0 && tP[0].y < SCR_Y-1) {
        if(tP[0].x < tP[2].x)
            dessinerLigne(screen, tP[0].x, tP[2].x, tP[0].y, color, isWireFrame);
        else
            dessinerLigne(screen, tP[2].x, tP[0].x, tP[0].y, color, isWireFrame);
    } else {
        a1 = (tP[0].x - tP[2].x)/((float)tP[0].y - tP[2].y);
        a2 = (tP[1].x - tP[2].x)/((float)tP[1].y - tP[2].y);
        for(int y = MIN(SCR_Y-1, tP[2].y); y > MAX(0, tP[1].y); y--) {
            x1 = (int)(- a1 * yP);
            x2 = (int)(- a2 * yP);

            if( x1 < x2 ) dessinerLigne(screen, x1 + tP[2].x, x2 + tP[2].x, y, color, isWireFrame);
            else          dessinerLigne(screen, x2 + tP[2].x, x1 + tP[2].x, y, color, isWireFrame);            

            yP ++;
        }
    }
}

/*
 * Initialiser SDL : renvoie la surface principale
 **/
SDL_Surface* SDL_initialiser() {
    SDL_Surface * screen;

    cout <<"Initializing SDL." << endl;
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)< 0) {
        cout <<"Could not initialize SDL:" << SDL_GetError() << endl;
        SDL_Quit();
    } else {
        cout << "Audio & Video initialized correctly" << endl;
    }

    screen = SDL_SetVideoMode(SCR_X, SCR_Y, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( screen == NULL ) {
        cerr << "Impossible de passer en "<<SCR_X << "x" << SCR_Y <<" : " << SDL_GetError() <<endl;
        SDL_Quit();
    }
    cout << "initialisation terminée" <<endl;

    return screen;
}

void flouterEcran(SDL_Surface* screen, signed char* tabRandom) {
    int x, y, color;
    SDL_Surface* screenTEMP = SDL_ConvertSurface(screen, screen->format, SDL_SWSURFACE); // copie de la surface pour éviter de propager les diffusions

    for(x=10;x<SCR_X-10; x++)
        for(y=10;y<SCR_Y-10; y++) {
            color =  GetPixel(screenTEMP, x+tabRandom[x*y%100], y+tabRandom[x*y%100]);
            DrawPixel(screen, x, y, color);
        }

}

bool trierTriangle(const Triangle& d1, const Triangle& d2)
{
    return d1.distanceMoyenne() < d2.distanceMoyenne();
}

int main(int argc, char *argv[])
{
    SDL_Surface *screen;
    vector<Triangle> vectTriangle;
    Transformation transfo;
    float delta[3];
    SDL_Event event;
    int done = 0;
    unsigned int t=0, initTime = 0;
    int sleep;
    bool flouter = false, isWireframe = false, backfaceC=false;
    vector<Triangle>::iterator it;

    bool mouvementAuto = true;

    delta[0] = 0;
    delta[1] = 0;
    delta[2] = 0;

    int trCount = 0, frameCount = 0;
    if(argc != 2) {
        cout <<"Usage : " <<argv[0] <<" Modele" <<endl;
        return EXIT_FAILURE;
    } else if(!readFromFile(argv[1], vectTriangle)) {
        cout <<"Erreur d'ouverture du fichier. Programme terminé" <<endl;
        return EXIT_FAILURE;
    }



    screen = SDL_initialiser();
    initTime = SDL_GetTicks();

    signed char tab[100], i;
    for(i=0;i<100;i++)
        tab[i]=random()%20-10;

    float rotX = 0.01, rotY=0.01;

    while(!done) { 
        frameCount ++;
        cout <<SDL_GetTicks() - t <<"ms (" << trCount << ")"<<endl;
        trCount = 0;
        sleep = 24 - (SDL_GetTicks() - t);
        SDL_Delay( (sleep > 0?sleep:1) );
        t = SDL_GetTicks();
        SDL_FillRect( screen, NULL, SDL_MapRGB(screen->format, 50, 50, 50));
        //      delta[0] = 0; //cos(t/600.0)/4.0;
        //     delta[1] = 0.2; //sin(t/600.0)/3.0;
        transfo = Transformation();
        if(mouvementAuto) {
            //      transfo.translate(delta);
            transfo.rotationX(t/6000.0);
            transfo.rotationZ(t/50000.0);
        } else {
            transfo.rotationX(rotX);
            transfo.rotationY(rotY);
        }

        for(it = vectTriangle.begin(); it != vectTriangle.end(); it++) {
            it->appliquerTransfo(transfo);
        }
        std::sort(vectTriangle.begin(), vectTriangle.end(), trierTriangle );
        for(it = vectTriangle.begin(); it != vectTriangle.end(); it++) {
            if(backfaceC || it->estAffiche()) { //DEBUG ME
                trCount ++;
                afficherTriangle(screen, (*it), isWireframe);
            }
            //  afficherVertex(screen, (*it));
        }

        if (flouter)
            flouterEcran(screen, tab);

        SDL_Flip(screen);

        if(SDL_PollEvent(&event)) switch(event.type) {

            case SDL_MOUSEMOTION:
                rotX = rotX + event.motion.yrel/60.0;
                rotY = rotY + event.motion.xrel/60.0;
                break;

            case SDL_KEYDOWN:
                cout << "scancode " << (int)(event.key.keysym.scancode) <<" unicode " << event.key.keysym.unicode <<endl;
                if( (int)(event.key.keysym.scancode) == 41) {// 'F'
                    flouter = !flouter;
                    break;
                }

                if( (int)(event.key.keysym.scancode) == 52) {// 'w'
                    isWireframe = !isWireframe;
                    break;
                }

                if( (int)(event.key.keysym.scancode) == 56) {// 'b'
                    backfaceC = !backfaceC;
                    break;
                }

                if( (int)(event.key.keysym.scancode) == 24) {// 'A' ... en azerty
                    mouvementAuto = !mouvementAuto;
                    break;
                }

                if(event.key.keysym.sym == SDLK_ESCAPE)
                    done=true;
                break;

            case SDL_QUIT:
                done = true;
                break;
        }
        while(SDL_PollEvent(&event)) { }
    }   // End Game Loop
    //  cout <<frameCount <<" frames in "<<(SDL_GetTicks()-initTime)<<" ms., mean fps : "<< int(frameCount / ((SDL_GetTicks()-initTime)/1000.0)) <<endl;;
    SDL_Quit();
}
