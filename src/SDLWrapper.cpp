#include "SDLWrapper.h"

#include <iostream>
#include <stdexcept>

/*
* Initialiser SDL : renvoie la surface principale
**/
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
  surface = SDL_GetWindowSurface(window);
  renderer = SDL_CreateSoftwareRenderer(surface);
  if (!renderer) {
    throw std::runtime_error(SDL_GetError());
  }

  m_screen.setInnerPointer(surface);

  if (m_screen.getInnerPointer() == nullptr) {
    throw std::runtime_error(SDL_GetError());
  } else {
    std::clog << "Video mode OK" << std::endl;
  }

  std::atexit(SDL_Quit);

  std::clog << "SDL initialization OK" << std::endl;
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
