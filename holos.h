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

#include "src/h_Defines.h"
#include "src/h_Debug.h"
#include "src/h_Core.h"

// platform

#ifdef H_LINUX
  //#include "src/h_Linux.h"
  #include "src/h_Platform_Linux.h"
#endif

#ifdef H_WIN32
  //#include "src/h_Win32.h"
  #include "src/h_Platform_Win32.h"
#endif

#include "src/h_Host.h"
#include "src/h_Instance.h"
//#include "src/h_Format.h"
#include "src/h_Parameter.h"
#include "src/h_Descriptor.h"

// host, instance, format

#ifdef H_EXE
  //#include "src/h_Exe.h"
  #include "src/h_Host_Exe.h"
  #include "src/h_Instance_Exe.h"
  #include "src/h_Format_Exe.h"
#endif

#ifdef H_LADSPA
  //#include "src/h_Ladspa.h"
  #include "src/h_Host_Ladspa.h"
  #include "src/h_Instance_Ladspa.h"
  #include "src/h_Format_Ladspa.h"
#endif

#ifdef H_VST
  //#include "src/h_Vst.h"
  #include "src/h_Host_Vst.h"
  #include "src/h_Instance_Vst.h"
  #include "src/h_Format_Vst.h"
#endif

#ifndef H_NOGUI
  #include "src/h_Editor.h"
#endif

//#include "src/h_Main.h"

//----------------------------------------------------------------------
#endif
