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
#ifndef h_Platform_Win32_impl_included
#define h_Platform_Win32_impl_included
//#ifdef h_Win32_included
//----------------------------------------------------------------------

//#include "lib/h_Globals.h"
//#include "lib/h_Utils.h"
#include "src/h_Globals.h"
#include "src/h_Utils.h"

#ifdef H_LIB

/*
  static_Debug.initialize() has not been called yet (it is called from
  main), so if trace() is trying to write to a log file, it crashes...
  we can move the static_Debug.initialize() into this DllMain
  function, but if so, we need to wrap this (and the 'normal' one in
  main(), so it is called only here
*/

__externc
BOOL APIENTRY
DllMain(HINSTANCE hModule, DWORD reason, LPVOID lpReserved)
{
  switch(reason)
  {
    case DLL_PROCESS_ATTACH:
      //trace("DLL_PROCESS_ATTACH"); // crashes
      static_WinInstance = hModule;
      break;
    //case DLL_PROCESS_DETACH:
    //  break;
    //case DLL_THREAD_ATTACH:
    //  break;
    //case DLL_THREAD_DETACH:
    //  break;
    //default:
    //  break;
  }
  return TRUE;
}

#endif

//----------------------------------------------------------------------

#ifndef H_NOGUI
// implemented in h_Window_Win32_impl.h
LRESULT CALLBACK h_eventproc_win32(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

//----------

h_Platform::h_Platform()
  {
    m_WinInstance = static_WinInstance;
        
    m_WinDepth = 24; //TODO: find real value...
    h_CreateUniqueName(m_WinClassName,(char*)"holos_win_",this);
    #ifndef H_NOGUI
    
    // something breaks the title bar icon (.exe)
    m_WinIcon = LoadIcon(m_WinInstance, "h_icon");
    m_WinCursor = NULL; // LoadCursor(NULL, IDC_ARROW);
    h_Memset(&m_WinClass,0,sizeof(m_WinClass));
    m_WinClass.style         = CS_HREDRAW | CS_VREDRAW;
    m_WinClass.lpfnWndProc   = &h_eventproc_win32;
    m_WinClass.hInstance     = m_WinInstance;
    m_WinClass.lpszClassName = m_WinClassName;
    m_WinClass.hCursor       = m_WinCursor;
    m_WinClass.hIcon         = m_WinIcon;
    m_WinClassAtom = RegisterClass(&m_WinClass);
    #endif

  }

//----------

h_Platform::~h_Platform()
  {
    #ifndef H_NOGUI
    UnregisterClass(m_WinClassName,m_WinInstance);
    #endif
  }

//----------

h_String h_Platform::getName(void)
  {
    return "win32";
  }

//----------------------------------------------------------------------
//#endif
#endif
