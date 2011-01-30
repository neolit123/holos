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
#ifndef h_Core_included
#define h_Core_included
//----------------------------------------------------------------------

class h_Platform;
class h_Format;

//----------------------------------------------------------------------

class h_Core
{
  public:
    h_Platform* m_Platform;
    h_Format*   m_Format;
  public:
    h_Core();
    ~h_Core();
    void initialize(void);
};

//----------

static h_Core static_Core;

//#define OS   static_Core.m_Platform
//#define APP  static_Core.m_Format
//#define PLUG static_Core.m_Format

//----------------------------------------------------------------------

// h_Platform

#ifdef H_LINUX
  #include "core/h_Linux.h"
#endif

#ifdef H_WIN32
  #include "core/h_Win32.h"
#endif

//----------

#include "core/h_Host.h"
#include "core/h_Parameter.h"
#include "core/h_Descriptor.h"
#include "core/h_Instance.h" // (h_Instance_Base.h)

//----------

// h_Instance
// h_Format

#ifdef H_EXE
  #include "core/h_Exe.h"
#endif

#ifdef H_LADSPA
  #include "core/h_Ladspa.h"
#endif

#ifdef H_VST
  #include "core/h_Vst.h"
#endif


//----------------------------------------------------------------------
#endif

