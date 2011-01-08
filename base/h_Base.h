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
#ifndef h_Base_included
#define h_Base_included
//----------------------------------------------------------------------

#include "base/h_Descriptor.h"
#include "base/h_Instance.h"
#include "base/h_Editor.h"
#include "base/h_Host.h"

template<class t_Descriptor,class t_Instance, class t_Editor>
class h_Base
{
  public:
    //h_Base() {}
    //~h_Base() {}
    inline h_Descriptor* createDescriptor(h_Host* a_Host)           { return new t_Descriptor(a_Host); }
    inline h_Instance*   createInstance(h_Descriptor* a_Descriptor) { return new t_Instance(a_Descriptor); }
    inline h_Editor*     createEditor(h_Instance* a_Instance)       { return new t_Editor(a_Instance); }
};

#define H_BASE(D,I,E) static h_Base<D,I,E> static_Base;
#define H_PLUGIN      H_BASE
#define H_APPLICATION H_BASE

//----------------------------------------------------------------------
#endif
