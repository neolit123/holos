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
#ifndef h_Core_impl_included
#define h_Core_impl_included
//#ifdef h_Core_included
//----------------------------------------------------------------------

#include "src/h_Core.h"

#include "src/h_Platform.h"
#include "src/h_Format.h"

/*
  this is called once, just after the dll/exe has been loaded into memory,
  during initialization of static variables (static_Core)
  we don't do much here, because the order of initialization for static
  variables can be a little uncertain, and we need to be sure that some
  other functions have been called (DllMain), so we have everything we
  need to proceed..
*/

h_Core::h_Core()
  {
    m_Initialized = false;
    m_Platform  = H_NULL;
    m_Format = H_NULL;
  }

//----------

/*
  this is also called once, as ome of the last things happening, when the
  exe/dll is unloaded from memory.
*/

h_Core::~h_Core()
  {
    if (m_Initialized)
    {
      // cppcheck: safe to delete NULL ptr
      if (m_Platform) delete m_Platform;
      if (m_Format) delete m_Format;
    }
  }

//----------

/*
  this is where we set everything in motion. this can be called multiple times,
  each time a new instance is created (vst).
*/

void h_Core::initialize(void)
  {
    if (!m_Initialized)
    {
      m_Platform  = new h_Platform();
      m_Format = new h_Format();
      m_Initialized = true;
    }
  }

//----------------------------------------------------------------------
//#endif
#endif
