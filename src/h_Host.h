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
#ifndef h_Host_included
#define h_Host_included
//----------------------------------------------------------------------

#include "src/h_String.h"

class h_Host_Base
{
  public:
    h_Host_Base() {}
    virtual ~h_Host_Base() {}
    virtual h_String getName(void) { return ""; }
};

//----------------------------------------------------------------------

#ifdef H_EXE
  #include "src/h_Host_Exe.h"
#endif

#ifdef H_LADSPA
  #include "src/h_Host_Ladspa.h"
#endif

#ifdef H_VST
  #include "src/h_Host_Vst.h"
#endif

//----------------------------------------------------------------------
#endif
