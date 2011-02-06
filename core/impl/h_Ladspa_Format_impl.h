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
#ifndef h_Ladspa_Format_impl_included
#define h_Ladspa_Format_impl_included
#ifdef h_Ladspa_included
//----------------------------------------------------------------------

h_Format::h_Format()
  {
  }

//----------

h_Format::~h_Format()
  {
  }

//----------------------------------------

h_Descriptor* h_Format::getDescriptor(void)
  {
    return &H_DESCRIPTOR;
  }

//----------

h_Instance* h_Format::createInstance(h_Descriptor* a_Descriptor)
  {
    return new H_INSTANCE(a_Descriptor);
  }

//----------------------------------------

int h_Format::entrypoint(void* a_Ptr)
  {

    h_Descriptor* descriptor = getDescriptor();
    h_Instance* instance = createInstance(descriptor);
//    if (descriptor->m_Flags & df_HasEditor)
//    {
//      instance->do_OpenEditor();
//      //instance->eventLoop();
//      instance->do_CloseEditor();
//    }
    delete instance;
    return 0;
  }


//----------------------------------------------------------------------
#endif
#endif

