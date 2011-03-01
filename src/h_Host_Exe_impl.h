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
#ifndef h_Host_Exe_impl_included
#define h_Host_Exe_impl_included
//#ifdef h_Exe_included
//----------------------------------------------------------------------

h_Host::h_Host()
: h_Host_Base()
  {
  }

//----------

h_Host::~h_Host()
  {
  }

//----------

h_String h_Host::getName(void)
  {
    return "exe";
  }

//----------------------------------------------------------------------
//#endif
#endif
