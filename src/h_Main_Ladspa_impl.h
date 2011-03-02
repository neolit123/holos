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
#ifndef h_Main_Ladspa_impl_included
#define h_Main_Ladspa_impl_included
//#ifdef h_Ladspa_included
//----------------------------------------------------------------------
/*
  looks like jost (and qtractor) doesn't see the exported function.
  'nm holos_ladspa_debug.so' shows a line like this:
    000057bf T ladspa_descriptor
  jost prints out:
  'You are trying to load a shared library ?'
  so it seems the .so isn't loading...
*/

//#include <stdio.h> // printf (for testing)

__externc __dllexport
const LADSPA_Descriptor* ladspa_descriptor(unsigned long Index)
{
  //printf("ladspa_descriptor!!!\n"); // testing
  if (Index>0) return H_NULL;
  static_Debug.initialize();
  static_Core.initialize();
  return static_Core.m_Format->entrypoint(H_NULL);
}

//----------------------------------------------------------------------
//#endif
#endif
