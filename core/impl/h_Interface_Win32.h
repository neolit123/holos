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
#ifndef h_Interface_Win32_included
#define h_Interface_Win32_included
//----------------------------------------------------------------------

#include "core/h_Platform.h"
#include "lib/h_Utils.h"

#include <windows.h>

// implemented in gui/impl/h_Window_Win32.h
LRESULT CALLBACK h_eventproc_win32(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class h_Interface_Win32 : public h_Platform
{
  private:
    HINSTANCE m_WinInstance;
    WNDCLASS  m_WinClass;
    char      m_WinClassName[256];
    ATOM      m_WinClassAtom;
    HCURSOR   m_WinCursor;
    HICON     m_WinIcon;
    int       m_WinDepth;

  public:
    inline char* getWinClassName(void) { return m_WinClassName; }
    inline ATOM  getWinClassAtom(void) { return m_WinClassAtom; }
    inline int   getWinDepth(void)     { return m_WinDepth; }

  public:

    h_Interface_Win32()
    : h_Platform()
      {
        UnregisterClass(m_WinClassName,m_WinInstance);
      }

    virtual ~h_Interface_Win32()
      {
      }

    virtual void initialize(void)
      {
        h_Platform::initialize();
        // register window class
        h_CreateUniqueName(m_WinClassName,(char*)"holos_window_",this);
        m_WinInstance = getWinInstance();
        m_WinDepth    = 24; //TODO: find real value...
        m_WinIcon     = LoadIcon(m_WinInstance,"holos_icon");
        m_WinCursor   = LoadCursor(NULL,IDC_ARROW);
        h_Memset(&m_WinClass,0,sizeof(m_WinClass));
        m_WinClass.style         = CS_HREDRAW | CS_VREDRAW;
        m_WinClass.lpfnWndProc   = &h_eventproc_win32;
        m_WinClass.hInstance     = m_WinInstance;
        m_WinClass.lpszClassName = m_WinClassName;
        m_WinClass.hCursor       = m_WinCursor;
        m_WinClass.hIcon         = m_WinIcon;
        m_WinClassAtom           = RegisterClass(&m_WinClass);
      }


};

//----------------------------------------------------------------------

typedef h_Interface_Win32 h_Interface;

//----------------------------------------------------------------------
#endif
