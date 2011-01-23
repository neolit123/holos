#ifdef h_Win32_included
#ifndef h_Win32_impl_included
#define h_Win32_impl_included
//----------------------------------------------------------------------

#include "lib/h_Globals.h"

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

h_Platform::h_Platform()
  {
    m_WinInstance = static_WinInstance;
  }

//----------

h_Platform::~h_Platform()
  {
  }

//----------------------------------------------------------------------
#endif
#endif
