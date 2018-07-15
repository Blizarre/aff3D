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

#ifndef __SURFACEWRAPPER_H_456124654

#define __SURFACEWRAPPER_H_456124654

#include <SDL2/SDL.h>
#include <memory>

/*
   This class is a Wrapper for the SDL_Surface pointer returned by a call to 
   SDL_GetWindowSurface. It do not own the pointer to the SDL_Surface.
*/
class SurfaceWrapper {
public:
  SurfaceWrapper(SDL_Surface* surface): m_SDLSurface(surface) { }

  inline int getWidth() { return m_SDLSurface->w; }
  inline int getHeight() { return m_SDLSurface->h; }

  inline Uint32 getColor(const Uint8 r, const Uint8 g, const Uint8 b) const {
    return SDL_MapRGB(m_SDLSurface->format, r, g, b);
  }

  // fill the surface with the color (r, g, b)
  void fill(const Uint8 r, const Uint8 g, const Uint8 b);

  // return the pixel value at index (x,y). Doesn't do any bound checking, and
  // may read/write at an invalid location
  inline Uint32 &pixel(const int x, const int y) const {
    Uint32 *bufp;

    bufp = (Uint32 *)m_SDLSurface->pixels + y * m_SDLSurface->pitch / 4 + x;
    return *bufp;
  }

  // Locking of the SDL Surface: Has to be called before any access (by
  // rasterizer or any other mean)
  void lockSurface() { SDL_LockSurface(m_SDLSurface); }

  // Unlocking of the SDL Surface: Has to be called after any access (by
  // rasterizer or any other mean)
  void unLockSurface() { SDL_UnlockSurface(m_SDLSurface); }

  inline SDL_Surface *getInnerPointer() const { return m_SDLSurface; }

protected:
  SDL_Surface* m_SDLSurface;
};

#endif
