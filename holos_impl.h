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
#ifndef holos_impl_included
#define holos_impl_included
#ifdef holos_h_included
//----------------------------------------------------------------------

// select default implementations if we don't provide our own
// sub-classes

  #ifndef H_DESCRIPTOR
    #define H_DESCRIPTOR h_Descriptor
  #endif

  #ifndef H_INSTANCE
    #define H_INSTANCE h_Instance
  #endif

  //#ifndef H_EDITOR
  //  #define H_EDITOR h_Editor
  //#endif

//----------------------------------------------------------------------

//#ifndef H_NOSRC

  #include "src/h_Debug_impl.h"
  #include "src/h_Core_impl.h"

  //----------

  //#include "src/h_Platform_impl.h"

  #ifdef H_LINUX
    #include "src/h_Platform_Linux_impl.h"
  #endif

  #ifdef H_WIN32
    #include "src/h_Platform_Win32_impl.h"
  #endif

  //#include "src/h_Format_impl.h"

  //----------

  //#include "src/h_Host_impl.h"
  //#include "src/h_Parameter_impl.h"
  //#include "src/h_Descriptor_impl.h"
  //#include "src/h_Instance_impl.h"

  //#ifndef H_NOGUI
  //  #include "src/h_Editor_impl.h"
  //#endif

  //#include "src/h_Main_impl.h"

  //----------

  #ifdef H_EXE
    #include "src/h_Host_Exe_impl.h"
    #include "src/h_Format_Exe_impl.h"
    #include "src/h_Instance_Exe_impl.h"
    #include "src/h_Main_Exe_impl.h"
  #endif

  #ifdef H_LADSPA
    #include "src/h_Host_Ladspa_impl.h"
    #include "src/h_Format_Ladspa_impl.h"
    #include "src/h_Instance_Ladspa_impl.h"
    #include "src/h_Main_Ladspa_impl.h"
  #endif

  #ifdef H_VST
    #include "src/h_Host_Vst_impl.h"
    #include "src/h_Format_Vst_impl.h"
    #include "src/h_Instance_Vst_impl.h"
    #include "src/h_Main_Vst_impl.h"
  #endif

//#endif

//----------------------------------------------------------------------
#endif
#endif
