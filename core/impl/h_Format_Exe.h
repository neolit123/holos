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
#ifndef h_Format_Exe_included
#define h_Format_Exe_included
//----------------------------------------------------------------------

#include "core/h_Interface.h"

class h_Format_Exe : public h_Interface
{
  public:

    h_Format_Exe()
    : h_Interface()
      {
      }

    virtual ~h_Format_Exe()
      {
      }

    virtual void initialize(void)
      {
        h_Interface::initialize();
      }

//    virtual void* entrypoint(void* a_Ptr)
//      {
//        trace("entrypoint");
//        return H_NULL;
//      }

};

//----------------------------------------------------------------------

typedef h_Format_Exe h_Format;

#define H_MAIN "core/impl/h_Main_Exe.h"

//----------------------------------------------------------------------
#endif
