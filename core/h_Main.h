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
#ifndef h_Main_included
#define h_Main_included
//----------------------------------------------------------------------

// not used?
// H_MAIN set directly to implementation in h_Format...

#ifdef H_EXE
  #include "core/impl/h_Main_Exe.h"
#endif

#ifdef H_LADSPA
  #include "core/impl/h_Main_Ladspa.h"
#endif

#ifdef H_VST
  #include "core/impl/h_Main_Vst.h"
#endif

//----------------------------------------------------------------------
#endif











//
//#ifndef h_Main_included
//#define h_Main_included
////----------------------------------------------------------------------
//
//#include "h_Types.h"
//
//#ifdef H_EXE
//
//  #define H_MAIN(_D,_I,_E)                    \
//    H_MAIN_INIT(_D,_I,_E)                     \
//    int main(void)                            \
//    {                                         \
//      H_CORE.initialize();                    \
//      void* res = H_FORMAT->entrypoint(NULL); \
//      return *(int*)res;                      \
//    }
//
///*
//  #define H_MAIN(_D,_I)                       \
//    H_MAIN_INIT(_D,_I)                        \
//    int main(void)                            \
//    {                                         \
//      H_CORE.initialize();                    \
//      void* res = H_FORMAT->entrypoint(NULL); \
//      return *(int*)res;                      \
//    }
//*/
//
//#endif // H_EXE
//
////----------------------------------------------------------------------
//
//#ifdef H_LADSPA
//
//  #define H_MAIN(_D,_I,_E)                    \
//    H_MAIN_INIT(_D,_I,_E)                     \
//    __externc __dllexport                     \
//    const LADSPA_Descriptor*                  \
//    ladspa_descriptor(unsigned long Index)    \
//    {                                         \
//      if (Index>0) return NULL;               \
//      H_CORE.initialize();                    \
//      void* res = H_FORMAT->entrypoint(NULL); \
//      return (LADSPA_Descriptor*)res;         \
//    }
//
//#endif // H_LADSPA
//
////----------------------------------------------------------------------
//
////  typedef audioMasterCallback __may_alias audioMasterCallback_a;
//
//#ifdef H_VST
//
//  #ifdef H_LINUX
//    #define _H_ASM_MAIN_SYMBOL asm ("main");
//  #endif
//  #ifdef H_WIN32
//    #define _H_ASM_MAIN_SYMBOL asm ("_main");
//  #endif
//
//  //---
//
//  // a.k.a. createEffectInstance
//  AEffect*  main_plugin(audioMasterCallback audioMaster) _H_ASM_MAIN_SYMBOL
//  #define   main main_plugin
//
//  //---
//
//  // note here we init
//
//  #define H_MAIN(_D,_I,_E)                                      \
//    H_MAIN_INIT(_D,_I,_E)                                       \
//    AEffect* main(audioMasterCallback audioMaster)              \
//    {                                                           \
//      if (!audioMaster(0,audioMasterVersion,0,0,0,0)) return 0; \
//      H_CORE.initialize();                                      \
//      void* res = H_FORMAT->entrypoint( *(void_ptr_a*)&audioMaster ); \
//      return (AEffect*)res; \
//    }
//
//#endif // H_VST
//
////----------------------------------------------------------------------
//#endif
