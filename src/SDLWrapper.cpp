#include "SDLWrapper.h"

#include <iostream>
#include <stdexcept>

SDLWrapper::SDLWrapper(size_t width, size_t height) {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    throw std::runtime_error(SDL_GetError());
  } else {
    std::clog << "Audio & Video modules initialized correctly" << std::endl;
  }

  SDL_Surface *surface;

  /* Create window and renderer for given surface */
  window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, width, height, 0);
  if (!window) {
    throw std::runtime_error(SDL_GetError());
  }

  // A surface returned by SDL_GetWindowSurface must not be freed
  surface = SDL_GetWindowSurface(window);
  if (!surface) {
    throw std::runtime_error(SDL_GetError());
  }

  renderer = SDL_CreateSoftwareRenderer(surface);
  if (!renderer) {
    throw std::runtime_error(SDL_GetError());
  }
  std::clog << "Video mode OK" << std::endl;

  m_screen.reset(new SurfaceWrapper(surface));

  std::clog << "SDL initialization OK" << std::endl;
}

SDLWrapper::~SDLWrapper() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void SDLWrapper::processEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {

    case SDL_MOUSEMOTION:
      if (m_onMouseMove)
        m_onMouseMove(event.motion.x, event.motion.y);
      break;

    case SDL_KEYDOWN:
      std::clog << "scancode " << (int)(event.key.keysym.sym) << std::endl;
      {
        auto itFind = m_keyboardEventBindings.find(event.key.keysym.sym);
        if (itFind != m_keyboardEventBindings.end())
          itFind->second();
      }
      break;

    case SDL_QUIT:
      if (m_quitEvent)
        m_quitEvent();
    }
  }
}
