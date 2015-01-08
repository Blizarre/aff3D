#include "SDLWrapper.h"

#include <exception>
#include <iostream>

/*
* Initialiser SDL : renvoie la surface principale
**/
SDLWrapper::SDLWrapper(size_t width, size_t height) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)< 0) {
        throw std::exception(SDL_GetError());
    }
    else {
        std::clog << "Audio & Video modules initialized correctly" << std::endl;
    }

    m_screen.setInnerPointer(SDL_SetVideoMode(static_cast<int>(width), static_cast<int>(height), 32, SDL_SWSURFACE | SDL_ANYFORMAT));
    
    if (m_screen.getInnerPointer() == nullptr) {
        throw std::exception(SDL_GetError());
    }
    else
    {
        std::clog << "Video mode set correctly";
    }

    std::atexit(SDL_Quit);

    std::clog << "SDL initialization OK" << std::endl;
}


void SDLWrapper::processEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {

        case SDL_MOUSEMOTION:
            if (m_onMouseMove)
                m_onMouseMove(event.motion.x, event.motion.y);
            break;

        case SDL_KEYDOWN:
            std::clog << "scancode " << (int)(event.key.keysym.sym) << " unicode " << event.key.keysym.unicode << std::endl;
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