#include "surfaceWrapper.h"

void SurfaceWrapper::fill(const Uint8 r, const Uint8 g, const Uint8 b) {
  SDL_FillRect(m_SDLSurface, NULL, getColor(r, g, b));
}
