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
#ifndef h_Main_Vst_impl_included
#define h_Main_Vst_impl_included
//#ifdef h_Vst_included
//----------------------------------------------------------------------

#include "extern/vst/aeffect.h"
#include "extern/vst/aeffectx.h"
//#include "extern/vst/vstfxstore.h"

//#include "core/source/h_Host_Vst_impl.h"
//#include "core/source/h_Instance_Vst_impl.h"
//#include "core/source/h_Format_Vst_impl.h"

//
//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------
//

#ifdef H_LINUX
  #define _H_ASM_MAIN_SYMBOL asm ("main");
#endif
#ifdef H_WIN32
  #define _H_ASM_MAIN_SYMBOL asm ("_main");
#endif

//----------

// a.k.a. createEffectInstance
AEffect*  main_plugin(audioMasterCallback audioMaster) _H_ASM_MAIN_SYMBOL
#define   main main_plugin

//----------

AEffect* main(audioMasterCallback audioMaster)
{
  static_Debug.initialize();
  static_Core.initialize();
  if (!audioMaster(0,audioMasterVersion,0,0,0,0)) return 0;
  return static_Core.m_Format->entrypoint(audioMaster);
}

//TODO:
// be sure we handle multiple instances properly..
// main() is called for each instance
// (and for the initial enumeration)

//----------------------------------------------------------------------
//#endif
#endif

