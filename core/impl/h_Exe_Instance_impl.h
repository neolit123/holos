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
#ifndef h_Exe_Instance_impl_included
#define h_Exe_Instance_impl_included
#ifdef h_Exe_included
//----------------------------------------------------------------------

h_Instance::h_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
//: h_Instance_Base(a_Descriptor)
  {
    m_Host = a_Host;
    m_Descriptor = a_Descriptor;
    m_EditorRect = m_Descriptor->m_EditorRect;// h_Rect(0,0,320,240);
    m_EditorIsOpen = false;
  }

//----------

h_Instance::~h_Instance()
  {
  }


//----------

void h_Instance::initParameters(void)
  {
  }

//----------

void h_Instance::prepareParameters(void)
  {
  }


//----------------------------------------------------------------------
#endif
#endif
