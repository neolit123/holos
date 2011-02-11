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
#ifndef holos_h_included
#define holos_h_included
//----------------------------------------------------------------------

#include "h/h_Defines.h"
#include "h/h_Debug.h"
#include "h/h_Core.h"

// platform

#ifdef H_LINUX
  #include "h/h_Linux.h"
#endif

#ifdef H_WIN32
  #include "h/h_Win32.h"
#endif


#include "h/h_Host.h"
//#include "h/h_Format.h"
#include "h/h_Instance.h"

#include "h/h_Parameter.h"
#include "h/h_Descriptor.h"

// host, instance, format

#ifdef H_EXE
  #include "h/h_Exe.h"
#endif

#ifdef H_LADSPA
  #include "h/h_Ladspa.h"
#endif

#ifdef H_VST
  #include "h/h_Vst.h"
#endif

#include "h/h_Editor.h"
//#include "h/h_Main.h"

//----------------------------------------------------------------------

//#include "lib/h_Defines.h"
//#include "debug/h_Debug.h"
//#include "core/h_Core.h"
//
//// platform
//
//#ifdef H_LINUX
//  #include "core/h_Linux.h"
//#endif
//
//#ifdef H_WIN32
//  #include "core/h_Win32.h"
//#endif
//
//
//#include "core/h_Host.h"
////#include "core/h_Format.h"
//#include "core/h_Instance.h"
//
//#include "core/h_Parameter.h"
//#include "core/h_Descriptor.h"
//
//// host, instance, format
//
//#ifdef H_EXE
//  #include "core/h_Exe.h"
//#endif
//
//#ifdef H_LADSPA
//  #include "core/h_Ladspa.h"
//#endif
//
//#ifdef H_VST
//  #include "core/h_Vst.h"
//#endif
//
//#include "core/h_Editor.h"
////#include "core/h_Main.h"


//----------------------------------------------------------------------
#endif
