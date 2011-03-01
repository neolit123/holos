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
#ifndef h_Platform_Win32_included
#define h_Platform_Win32_included
//----------------------------------------------------------------------

#include <windows.h>
#include "src/h_Globals.h"
#include "src/h_String.h"

//----------------------------------------------------------------------

class h_Platform
{
  public:
    HINSTANCE m_WinInstance;
    char      m_WinClassName[H_MAX_STRINGSIZE];
    ATOM      m_WinClassAtom;
    HCURSOR   m_WinCursor;
    HICON     m_WinIcon;
    WNDCLASS  m_WinClass;
    int       m_WinDepth;
  public:
    h_Platform();
    ~h_Platform();
    h_String getName(void);
};

//----------------------------------------------------------------------
#endif

