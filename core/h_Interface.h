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
#ifndef h_Interface_included
#define h_Interface_included
//----------------------------------------------------------------------

#ifdef H_LINUX
  #include "core/impl/h_Interface_Linux.h"
#endif

#ifdef H_WIN32
  #include "core/impl/h_Interface_Win32.h"
#endif

//----------------------------------------------------------------------

//class h_Interface_Base
//{
//  public:
//    h_Interface_Base() {}
//    virtual ~h_Interface_Base() {}
//    virtual h_Interface_Data* getData(void) { return H_NULL; }
//};

//----------------------------------------------------------------------
#endif
