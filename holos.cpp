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
#ifndef holos_cpp_included
#define holos_cpp_included
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

  #include "cpp/h_Debug.cpp"
  #include "cpp/h_Core.cpp"

  //----------

  //#include "cpp/h_Platform.cpp"

  #ifdef H_LINUX
    #include "cpp/h_Platform_Linux.cpp"
  #endif

  #ifdef H_WIN32
    #include "cpp/h_Platform_Win32.cpp"
  #endif

  //#include "cpp/h_Format.cpp"

  //----------

  //#include "cpp/h_Host.cpp"
  //#include "cpp/h_Parameter.cpp"
  #include "cpp/h_Descriptor.cpp"
  //#include "cpp/h_Instance.cpp"
  #include "cpp/h_Editor.cpp"
  //#include "cpp/h_Main.cpp"

  //----------

  #ifdef H_EXE
    #include "cpp/h_Host_Exe.cpp"
    #include "cpp/h_Format_Exe.cpp"
    #include "cpp/h_Instance_Exe.cpp"
    #include "cpp/h_Main_Exe.cpp"
  #endif

  #ifdef H_LADSPA
    #include "cpp/h_Host_Ladspa.cpp"
    #include "cpp/h_Format_Ladspa.cpp"
    #include "cpp/h_Instance_Ladspa.cpp"
    #include "cpp/h_Main_Ladspa.cpp"
  #endif

  #ifdef H_VST
    #include "cpp/h_Host_Vst.cpp"
    #include "cpp/h_Format_Vst.cpp"
    #include "cpp/h_Instance_Vst.cpp"
    #include "cpp/h_Main_Vst.cpp"
  #endif

//#endif


//----------------------------------------------------------------------
////#ifndef H_NOSRC
//
//  #include "debug/source/h_Debug.cpp"
//  #include "core/source/h_Core.cpp"
//
//  //----------
//
//  //#include "core/source/h_Platform.cpp"
//
//  #ifdef H_LINUX
//    #include "core/source/h_Platform_Linux.cpp"
//  #endif
//
//  #ifdef H_WIN32
//    #include "core/source/h_Platform_Win32.cpp"
//  #endif
//
//  //#include "core/source/h_Format.cpp"
//
//  //----------
//
//  //#include "core/source/h_Host.cpp"
//  //#include "core/source/h_Parameter.cpp"
//  #include "core/source/h_Descriptor.cpp"
//  //#include "core/source/h_Instance.cpp"
//  #include "core/source/h_Editor.cpp"
//  //#include "core/source/h_Main.cpp"
//
//  //----------
//
//  #ifdef H_EXE
//    #include "core/source/h_Host_Exe.cpp"
//    #include "core/source/h_Format_Exe.cpp"
//    #include "core/source/h_Instance_Exe.cpp"
//    #include "core/source/h_Main_Exe.cpp"
//  #endif
//
//  #ifdef H_LADSPA
//    #include "core/source/h_Host_Ladspa.cpp"
//    #include "core/source/h_Format_Ladspa.cpp"
//    #include "core/source/h_Instance_Ladspa.cpp"
//    #include "core/source/h_Main_Ladspa.cpp"
//  #endif
//
//  #ifdef H_VST
//    #include "core/source/h_Host_Vst.cpp"
//    #include "core/source/h_Format_Vst.cpp"
//    #include "core/source/h_Instance_Vst.cpp"
//    #include "core/source/h_Main_Vst.cpp"
//  #endif
//
////#endif
//----------------------------------------------------------------------
#endif
#endif
