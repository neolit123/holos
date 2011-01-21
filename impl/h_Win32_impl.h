#ifdef h_Win32_included
#ifndef h_Win32_impl_included
#define h_Win32_impl_included
//----------------------------------------------------------------------

#include "lib/h_Globals.h"

#ifdef H_LIB
  //DllMain
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
