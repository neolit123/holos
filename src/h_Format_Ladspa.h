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
#ifndef h_Format_Ladspa_included
#define h_Format_Ladspa_included
//----------------------------------------------------------------------

//#define MAX_LADSPA_PORTS 256

#include "../extern/ladspa/ladspa.h"

class h_Format
{
  private:
    h_Platform*           m_Platform;
    h_Descriptor*         m_Descriptor;
    LADSPA_Descriptor     m_LadspaDescr;
    char*                 m_PortNames[MAX_LADSPA_PORTS];
    LADSPA_PortDescriptor m_PortDesc[MAX_LADSPA_PORTS];
    LADSPA_PortRangeHint  m_PortHint[MAX_LADSPA_PORTS];

  public:
    h_Format();
    ~h_Format();
    h_Descriptor*       createDescriptor(void);
    h_Instance*         createInstance(h_Host* a_Host,h_Descriptor* a_Descriptor);
    h_String            getName(void) { return "ladspa"; }
    LADSPA_Descriptor*  entrypoint(void* a_Ptr);
    LADSPA_Handle       lad_instantiate(const LADSPA_Descriptor* Descriptor, unsigned long SampleRate);

};


//----------------------------------------------------------------------
#endif

