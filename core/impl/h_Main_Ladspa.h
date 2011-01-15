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
#ifndef h_Main_Ladspa_included
#define h_Main_Ladspa_included
//----------------------------------------------------------------------



//----------------------------------------------------------------------
#endif

//#ifdef H_LADSPA
//
//  #define H_MAIN(_D,_I,_E)
//    H_MAIN_INIT(_D,_I,_E)
//    __externc __dllexport
//    const LADSPA_Descriptor*
//    ladspa_descriptor(unsigned long Index)
//    {
//      if (Index>0) return NULL;
//      H_CORE.initialize();
//      void* res = H_FORMAT->entrypoint(NULL);
//      return (LADSPA_Descriptor*)res;
//    }
//
//#endif // H_LADSPA
