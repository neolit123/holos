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

#include <windows.h>
#include "lib/h_Utils.h"

// implemented in gui/impl/h_Window_Win32.h
LRESULT CALLBACK h_eventproc_win32(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//----------------------------------------------------------------------

struct h_Interface_Data
{
  HINSTANCE m_WinInstance;
  char      m_WinClassName[256];
  int       m_WinDepth;
  HICON     m_WinIcon;
  HCURSOR   m_WinCursor;
  ATOM      m_WinClassAtom;
};

//----------------------------------------------------------------------

class h_Interface  : public h_Interface_Base
{
  private:
    h_Interface_Data m_Data;
  public:
    virtual h_Interface_Data* getData(void) { return &m_Data; }

  public:

    h_Interface()
    : h_Interface_Base()
      {
        //printf("h_Interface\n");
        h_Platform_Data* pl_data = static_Core.getPlatform()->getData();
        m_Data.m_WinInstance = pl_data->m_WinInstance;
        h_CreateUniqueName(m_Data.m_WinClassName,(char*)"holos_window_",this);
        m_Data.m_WinDepth  = 24; //TODO: find real value...
        m_Data.m_WinIcon   = LoadIcon(m_Data.m_WinInstance,"holos_icon");
        m_Data.m_WinCursor = LoadCursor(NULL,IDC_ARROW);
        WNDCLASS WinClass;
        h_Memset(&WinClass,0,sizeof(WinClass));
        WinClass.style         = CS_HREDRAW | CS_VREDRAW;
        WinClass.lpfnWndProc   = &h_eventproc_win32;
        WinClass.hInstance     = m_Data.m_WinInstance;
        WinClass.lpszClassName = m_Data.m_WinClassName;
        WinClass.hCursor       = m_Data.m_WinCursor;
        WinClass.hIcon         = m_Data.m_WinIcon;
        m_Data.m_WinClassAtom  = RegisterClass(&WinClass);
      }

    virtual ~h_Interface()
      {
      }

    //----------

};

//----------------------------------------------------------------------
#endif
