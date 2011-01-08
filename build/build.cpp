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

//#define H_DEBUG
//#undef H_DEBUG

#define H_DEBUG_LOG "holos_win.log"
#define H_DEBUG_CON
#define H_DEBUG_MEM
#define H_DEBUG_NEW

#include "holos.h"
#include "build/build_all.h"

//----------------------------------------------------------------------

class my_Descriptor : public h_Descriptor
{
  public:
    my_Descriptor(h_Host* a_Host)
    //: h_Descriptor(a_Host)
      {
        m_Name   = "holos";
        m_Author = "ccernn";
        m_Name   = "test plugin";
      }
    //virtual ~my_Descriptor()
    //  {
    //  }
};

//------------------------------

class my_Instance : public h_Instance
{
  public:
    my_Instance(h_Descriptor* a_Descriptor)
    : h_Instance(a_Descriptor)
      {
        trace("hello world!");
      }
    virtual ~my_Instance()
      {
      }
};

//------------------------------

class my_Editor : public h_Editor
{
  public:
    my_Editor(h_Instance* a_Instance)
    : h_Editor(a_Instance)
      {
      }
    virtual ~my_Editor()
      {
      }
};

//----------------------------------------------------------------------

H_PLUGIN(my_Descriptor,my_Instance,my_Editor)
#include H_MAIN

