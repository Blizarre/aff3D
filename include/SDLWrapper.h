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

#ifndef __SDLWRAPPER_H_456123165

#define __SDLWRAPPER_H_456123165

#include "surfaceWrapper.h"
#include <SDL2/SDL.h>
#include <functional>
#include <iostream>
#include <map>

class SDLWrapper {
public:
  SDLWrapper(int width, int height);

  unsigned int static getTicks() { return SDL_GetTicks(); }

  void flipBuffer() { SDL_UpdateWindowSurface(window); }

  SurfaceWrapper &getMainScreen() { return *m_screen; }

  /***
   * bind a key number to a std::function. When processEvent will be called,
   * this function will be called at every keypress of keyNum. Warning, only
   * one function can be bound to a keyNum. previous bindings for this keynum
   * will be discarded.
   **/
  void onKeyPress(unsigned char keyNum, std::function<void()> f) {
    m_keyboardEventBindings[keyNum] = f;
  }

  void onMouseMotion(std::function<void(size_t, size_t)> f) {
    m_onMouseMove = f;
  }

  void onQuitEvent(std::function<void()> f) { m_quitEvent = f; }

  void processEvents();

  ~SDLWrapper();

protected:
  std::map<Uint32, std::function<void(void)>> m_keyboardEventBindings;
  std::function<void(void)> m_quitEvent;
  std::function<void(size_t, size_t)> m_onMouseMove;

  std::unique_ptr<SurfaceWrapper> m_screen;
  SDL_Renderer *renderer;
  SDL_Window *window;
};

#endif /* __SDLWRAPPER_H_456123165 */
