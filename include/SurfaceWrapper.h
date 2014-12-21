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

#include <SDL.h>

class SurfaceWrapper
{
public:

    SurfaceWrapper()
    {
        m_SDLSurface = nullptr;
    }

    SurfaceWrapper(SDL_Surface * const surface)
    {
        m_SDLSurface = surface;
    }

    /* TODO: and what about assign copy operator? */
    SurfaceWrapper(const SurfaceWrapper & other)
    {
        m_SDLSurface = SDL_ConvertSurface(other.m_SDLSurface, other.m_SDLSurface->format, SDL_SWSURFACE);
    }

    inline Uint32 getColor(const Uint8 r, const Uint8 g, const Uint8 b) const
    {
        return SDL_MapRGB(m_SDLSurface->format, r, g, b);
    }

    void fill(const Uint8 r, const Uint8 g, const Uint8 b);

    inline Uint32 GetPixel(const int x, const int y) const
    {
        Uint32 *bufp;

        bufp = (Uint32 *)m_SDLSurface->pixels + y*m_SDLSurface->pitch / 4 + x;
        return *bufp;
    }

    inline void DrawPixel(const int x, const int y, const Uint32 color)
    {
        Uint32 *bufp;
        bufp = (Uint32 *)m_SDLSurface->pixels + y*m_SDLSurface->pitch / 4 + x;
        *bufp = color;
    }

    inline SDL_Surface * getInnerPointer() const {
        return m_SDLSurface;
    }

    /***
    * Change the pointer of the SDL_Surface. Will release the previous pointer if set.
    ***/
    void setInnerPointer(SDL_Surface * newSurface);



    virtual ~SurfaceWrapper();

protected:
    SDL_Surface * m_SDLSurface;
};

#endif
