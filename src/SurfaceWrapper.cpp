#include "SurfaceWrapper.h"

SurfaceWrapper::~SurfaceWrapper() {
    if (m_SDLSurface != nullptr)
        SDL_FreeSurface(m_SDLSurface);
}

/***
* Change the pointer of the SDL_Surface. Will release the previous pointer if set.
***/
void SurfaceWrapper::setInnerPointer(SDL_Surface * newSurface)
{
    if (m_SDLSurface != nullptr)
        SDL_FreeSurface(m_SDLSurface);
    m_SDLSurface = newSurface;
}


void SurfaceWrapper::fill(const Uint8 r, const Uint8 g, const Uint8 b) {
    SDL_FillRect(m_SDLSurface, NULL, getColor(r, g, b));
}