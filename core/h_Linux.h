/*
  Copyright (c) 2010 Tor-Helge Skei, Lubomir I. Ivanov et al

  This file is part of the Holos Library.
  http://holos.googlecode.com

  the Holos Library is free software: you can redistribute it and/or modify
  it under the terms of the Holos Library License, either version 1.0
  of the License, or (at your option) any later version.

  the Holos Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See LICENSE_HOLOS for more details.

  You should have received a copy of the Holos Library License
  If not, see <http://holos.googlecode.com/>.
*/
//----------------------------------------------------------------------
#ifndef h_Linux_included
#define h_Linux_included
//----------------------------------------------------------------------

//#include <windows.h>
//#include "lib/h_Globals.h"
#include "lib/h_String.h"

#ifndef H_NOGUI
  #include <X11/Xlib.h>
#endif


//----------------------------------------------------------------------

class h_Platform
{
  public:
    #ifndef H_NOGUI
    Display*  m_WinDisplay;
    Window    m_WinRoot;
    int       m_WinScreen;
    Visual*   m_WinVisual;
    int       m_WinDepth;
    Colormap  m_WinColormap;
    #endif
  public:
    h_Platform();
    ~h_Platform();
    h_String getName(void) { return "linux"; }
};

//----------------------------------------------------------------------
#endif
