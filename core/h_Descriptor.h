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
#ifndef h_Descriptor_Included
#define h_Descriptor_Included
//----------------------------------------------------------------------

#define df_None       0
#define df_HasEditor  1
#define df_IsSynth    2

#include "lib/h_Rect.h"

//----------------------------------------------------------------------

#ifdef H_EXE
  #include "core/impl/h_Descriptor_Exe.h"
#endif

#ifdef H_LADSPA
  #include "core/impl/h_Descriptor_Ladspa.h"
#endif

#ifdef H_VST
  #include "core/impl/h_Descriptor_Vst.h"
#endif

//----------------------------------------------------------------------

//class h_Descriptor : public h_Descriptor_Base
//{
//  public:
//    //h_Descriptor() {}
//    //virtual ~h_Descriptor() {}
//    const char* m_Name;
//    const char* m_Author;
//    const char* m_Product;
//    //
//    int     m_Flags;
//    h_Rect  m_EditorRect;
//
//};

//----------------------------------------------------------------------
#endif
