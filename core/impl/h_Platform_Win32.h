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
#include "lib/h_Globals.h"

//----------------------------------------------------------------------

#ifdef H_LIB

__externc
BOOL APIENTRY
DllMain(HINSTANCE hModule, DWORD reason, LPVOID lpReserved)
{
  switch(reason)
  {
    case DLL_PROCESS_ATTACH:
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

#endif // H_LIB

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class h_Platform_Win32
{
  private:
    HINSTANCE m_WinInstance;

  public:
    inline HINSTANCE getWinInstance(void) { return m_WinInstance; }

  public:

    h_Platform_Win32()
      {
      }

    virtual ~h_Platform_Win32()
      {
      }

    virtual void initialize(void)
      {
        m_WinInstance = static_WinInstance;
      }

};

//----------------------------------------------------------------------

typedef h_Platform_Win32 h_Platform;

//----------------------------------------------------------------------
#endif
